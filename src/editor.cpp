#include <core.hpp>
#include <cstdio>
#include <settings.hpp>
#include <level.hpp>
#include <string>
#include <fstream>
#include <world.hpp>
#include <ai.hpp>
#include <input.hpp>
#include <editor.hpp>

namespace game {
	using namespace std;

	ControllerFn ChuFuncs[10] {
		AttackOrRotate,
		AttackSW,
	};

	CellRef createRefDefault(Type type) {

		switch (type) {
			case Type::PLAYER:
				{
					return CellRef(type, Player { .Dir = X_AXIS });
				}
			case Type::CHU: 
				{
					return CellRef(type, Chu { .Dir = X_AXIS, .Func = ChuFuncs[0] });
				}
			case Type::SCENERY:
				{
					return CellRef(type, Scenery { .Type = TREE });
				}
			case Type::NEXTLEVELPORTAL:
				{
					return CellRef(type, NextLevelPortal { });
				}
			case Type::GRASS:
				{
					return CellRef(type, Grass { });
				}
			case Type::GOAT:
				{
					return CellRef(type, Goat { });
				}
			case Type::WATER:
				{
					return CellRef(type, Water { });
				}
			case Type::MOVEABLE:
				{
					return CellRef(type, Moveable { MoveableType::ROCK } );
				}
			case Type::SACI:
				{
					return CellRef(type, Saci { } );
				}
		}
	}

	ivec2 getCoordFromScreenPos(vec2 screenPos, u32 height, u32 width) {
		f32 x = (screenPos.x * width) / WIDTH;
		f32 y = height - (screenPos.y * height) / HEIGHT;
		return { x, y };
	}

	void writeNum(s32 val, char* buffer, u32& idx) {
		if (val < 0) {
			buffer[idx++] = '-';
			val = -val;
		}
		do {
			u8 digit = val % 10;
			buffer[idx++] = '0' + digit;
			val = val / 10;
		} while (val != 0);
		buffer[idx++] = ';';
	}

	void WriteLineEnt(CellRef* ref, ivec2 pos, char* buffer, u32& idx) {
		if (ref->type == Type::GRASS) {
			return;
		}

		writeNum(pos.x, buffer, idx);
		writeNum(pos.y, buffer, idx);
		writeNum(ref->type, buffer, idx);
		switch (ref->type) {
			case Type::CHU:
				{
					Chu chu = ref->chu;
					writeNum(chu.Dir.x, buffer, idx);
					writeNum(chu.Dir.y, buffer, idx);
					writeNum(0, buffer, idx);
					break;
				}
			case Type::MOVEABLE:
				{
					writeNum(ref->moveable.type, buffer, idx);
					break;
				}

			case Type::SACI:
				{
					writeNum(ref->saci.moveType, buffer, idx);
				}
			default:
				break;
		}
		buffer[idx++] = '\n';
	}

	u32 dist(ivec2 const& a, ivec2 const& b) {
		ivec2 v = abs(a - b);
		return v.x + v.y;
	}

	void deleteFromGrid(EditorContext& context, ivec2 pos) {
		CellRef* ref = context.level->grid.get(pos);
		if (ref->type == Type::GOAT) {
			vector<ivec2>& goatPositions = context.level->goatPos;
			int i = 0;
			for (; goatPositions[i] != pos && i < goatPositions.size(); i++);
			if (i == goatPositions.size()) return;
			for (; i + 1 < goatPositions.size(); i++) {
				goatPositions[i] = goatPositions[i + 1];
			}
			goatPositions.pop_back();
		}
		context.level->grid.remove(pos);
	}

	void sortByDist(vector<ivec2>& pos, ivec2 pp) {
		std::sort(pos.begin(), pos.end(), [pp](ivec2 const& lhs, ivec2 const& rhs){ return dist(lhs, pp) < dist(rhs, pp); });
	}

	void EditorController(EditorContext& context) {
		if (isKeyPressed(KEY_P)) {
			printf("place mode\n");
			context.state = EState::PLACING;
		} else if (isKeyPressed(KEY_A)) {
			context.state = EState::ASSIGN_FUNC;
		} else if (isKeyClicked(KEY_W)) {
			printf("save mode\n");
			context.state = EState::WRITING;
		} else if (isKeyClicked(KEY_O)) {
			printf("edit mode\n");
			context.state = EState::OPEN;
		} else if (isKeyClicked(KEY_S)) {
			printf("Solving\n");
			Level& level = *context.level;
			context.instructionIndex = 0;
			context.recordedInstructions = solveLevel(level);
			printf("Solveeeed\n");
		} else if (isKeyClicked(KEY_D)) {
			if (context.recordedInstructions.size() == 0) {
				printf("No instructions\n");
				return;
			}
			context.instructionIndex = context.instructionIndex % context.recordedInstructions.size();
			printf("Playing recorded instruction at %d / %lu\n", context.instructionIndex, context.recordedInstructions.size());
			processStep(*context.level, context.recordedInstructions[context.instructionIndex++]);
		}

		if (context.state == EState::PLACING) {
			s8 selection = getNumericKeyPressed();
			if (selection >= 0) {
				context.entId = static_cast<Type>(selection);
			}
			vec2 mousePos;
			if (isLeftMouseClicked(&mousePos)) {
				ivec2 coord = getCoordFromScreenPos(mousePos, context.level->grid.Height, context.level->grid.Width);

				deleteFromGrid(context, coord);

				CellRef ref = createRefDefault(context.entId);
				context.level->grid.set(coord.y, coord.x, ref);

				if (context.entId == Type::PLAYER) {
					context.level->playerPos = coord;
					ivec2 pp = context.level->playerPos;
					sortByDist(context.level->goatPos, pp);
				} else if (context.entId == Type::GOAT) {
					context.level->goatPos.push_back(coord);
					ivec2 pp = context.level->playerPos;
					sortByDist(context.level->goatPos, pp);
				}

			} else if (isRightMouseClicked(&mousePos)) {
				ivec2 coord = getCoordFromScreenPos(mousePos, context.level->grid.Height, context.level->grid.Width);
				CellRef* ref = context.level->grid.get(coord.y, coord.x);
				if (ref->type == Type::CHU) {
					Chu& chu = ref->chu;
					chu.Dir = { -chu.Dir.y, chu.Dir.x };
				}
			}
		}

		if (context.state == EState::WRITING) {
			char name[100] = "a.lvl";
			s8 inp = getNumericKeyPressed();
			if (inp == -1) {
				return;
			}
			// @attention scanf stops the program from working, need alternative
			name[0] = '0' + inp;

			printf("Saving file as %s\n", name);
			string n  = "levels/" + string(name);
			FILE* file = fopen(n.c_str(), "w");
			char levelBuffer[100000];
			u32 fileIndex = 0;
			for (u32 row = 0; row < context.level->grid.Height; row++) {
				for (u32 col = 0; col < context.level->grid.Width; col++) {
					WriteLineEnt(context.level->grid.get(row, col), ivec2( col, row ), levelBuffer, fileIndex);
				}
			}
			printf("Writing file\n");
			fwrite(levelBuffer, sizeof(char), fileIndex, file);
			printf("Wrote file\n");
			fclose(file);
			printf("Closed file\n");

			context.state = EState::PLACING;
		}

		if (context.state == EState::OPEN) {
			char name[100] = "a.lvl";
			s8 inp = getNumericKeyPressed();
			if (inp == -1) {
				return;
			}
			name[0] = '0' + inp;
			string fileName = "levels/" + string(name);
			if (context.level != nullptr) {
				delete context.level;
			}
			context.level = ReadLevel(fileName.c_str());
			context.state = EState::PLACING;

		}
	}

	Level* ReadLevel(string const& path) {
		printf("Reading %s\n", path.c_str());
		Level* level = new Level{
			.grid = { 10, 10 }
		};
		level->step = 0;
		level->flags = {};
		level->playerPos = { 0, 0 };
		level->levelName = path;

		string line;
		ifstream file(path);
		if (file.fail()) {
			fprintf(stderr, "Could not read Level\n");
				Player player {};
				level->grid.set(0, 0, CellRef(Type::PLAYER, player));
			return level;
		}

		while (getline(file, line)) {
			if (line.empty() || line[0] == '\n') {
				continue;
			} else {
				char* lineStr = (char*) line.c_str();

				char* token = strtok(lineStr, ";");
				assert(token != nullptr);
				u8 xi = stoi(token);
				token = strtok(nullptr, ";");
				assert(token != nullptr);
				u8 yi = stoi(token);

				u8 type = stoi(strtok(nullptr, ";"));
				switch (type) {
					case Type::PLAYER: // player 
						{
							Player player {};
							player.Dir = X_AXIS;
							level->grid.set(yi, xi, CellRef(Type::PLAYER, player));
							level->playerPos = { xi, yi };
							break;
						}
					case Type::CHU: // chu
						{
							Chu chu {};
							s8 dxi = stoi(strtok(nullptr, ";"));
							s8 dyi = stoi(strtok(nullptr, ";"));
							chu.Dir = { dxi, dyi };
							chu.Func = ChuFuncs[stoi(strtok(nullptr, ";"))];
							level->grid.set(yi, xi, CellRef(Type::CHU, chu));
							break;
						}
					case Type::SCENERY: // scenery
						{
							Scenery scenery { .Type = ScenearyType::TREE };
							level->grid.set(yi, xi, CellRef(Type::SCENERY, scenery));
							break;
						}
					case Type::NEXTLEVELPORTAL: // portal 
						{
							NextLevelPortal portal { };
							level->grid.set(yi, xi, CellRef(Type::NEXTLEVELPORTAL, portal));
							break;
						}
					case Type::GOAT:
						{
							Goat goat {};
							level->goatPos.push_back({xi, yi });
							level->grid.set(yi, xi, CellRef(Type::GOAT, goat));
							break;
						}
					case Type::WATER:
						{
							Water water {};
							level->grid.set(yi, xi, CellRef(Type::WATER, water));
							break;
						}
					case Type::MOVEABLE:
						{
							u8 moveableType = stoi(strtok(nullptr, ";"));
							Moveable moveable { .type = static_cast<MoveableType>(moveableType) };
							level->grid.set(yi, xi, CellRef(Type::MOVEABLE, moveable));
							break;
						}
					case Type::SACI:
						{
							u8 saciMoveType = stoi(strtok(nullptr, ";"));
							Saci saci { .moveType = static_cast<SaciMoveType>(saciMoveType)};
							level->saciPos.push_back({ xi, yi });
							level->grid.set(yi, xi, CellRef(Type::SACI, saci));
							break;
						}
					case Type::GRASS:
						{
						}
				}
				sortByDist(level->goatPos, level->playerPos);
			}
		}

		return level;
	}
}
