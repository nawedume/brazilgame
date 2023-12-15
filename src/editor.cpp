#include <core.hpp>
#include <cstdio>
#include <settings.hpp>
#include <level.hpp>
#include <string>
#include <fstream>
#include <variant>
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

	CellRef createRefDefault(EntId entId) {

		switch (entId) {
			case PLAYER:
				{
					return Player { .Dir = X_AXIS };
				}
			case CHU:
				{
					return Chu { .Dir = X_AXIS, .Func = ChuFuncs[0] };
				}
			case SCENERY:
				{
					return Scenery { .Type = TREE };
				}
			case NEXTLEVELPORTAL:
				{
					return NextLevelPortal { };
				}
			case GRASS:
				{
					return Grass { };
				}
			default:
				{
					fprintf(stderr, "No entity found with that ID\n");
					return Grass();
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
		if (holds_alternative<Grass>(*ref)) {
			return;
		}

		writeNum(pos.x, buffer, idx);
		writeNum(pos.y, buffer, idx);
		writeNum(ref->index(), buffer, idx);
		switch (ref->index()) {
			case 0: 
				{
					Player player = get<Player>(*ref);
					break;
				}
			case 1:
				{
					Chu chu = get<Chu>(*ref);
			printf(">> %d, %d\n", chu.Dir.x, chu.Dir.y);
					writeNum(chu.Dir.x, buffer, idx);
					writeNum(chu.Dir.y, buffer, idx);
					writeNum(0, buffer, idx);
					break;
				}
			case 2: // Tree
				{
					break;
				}
			case 4: // Portal
				{
					break;
				}
			default:
				break;
		}
		buffer[idx++] = '\n';
	}

	void EditorController(EditorContext& context) {
		if (isKeyPressed(KEY_P)) {
			context.state = EState::PLACING;
		} else if (isKeyPressed(KEY_A)) {
			context.state = EState::ASSIGN_FUNC;
		} else if (isKeyClicked(KEY_W)) {
			printf("save mode\n");
			context.state = EState::WRITING;
		} else if (isKeyClicked(KEY_O)) {
			printf("edit mode\n");
			context.state = EState::OPEN;
		}

		if (context.state == EState::PLACING) {
			s8 selection = getNumericKeyPressed();
			if (selection >= 0) {
				context.entId = static_cast<EntId>(selection);
			}
			vec2 mousePos;
			if (isLeftMouseClicked(&mousePos)) {
				ivec2 coord = getCoordFromScreenPos(mousePos, context.level->grid.Height, context.level->grid.Width);
				CellRef ref = createRefDefault(context.entId);
				context.level->grid.set(coord.y, coord.x, ref);

				if (context.entId == EntId::PLAYER) {
					context.level->playerPos = coord;
				}
			}
		}

		if (context.state == EState::WRITING) {
			char name[100] {'a', '.', 'l', 'v', 'l', '\0'};
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

		}
	}

	Level* ReadLevel(string const& path) {
		Level* level = new Level{
			.grid = { 10, 10 }
		};
		level->step = 0;
		level->flags = {};
		level->playerPos = { 0, 0 };

		string line;
		ifstream file(path);
		if (file.fail()) {
			fprintf(stderr, "Could not read Level\n");
			exit(1);
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
					case PLAYER: 
						{
							Player player {};
							player.Dir = X_AXIS;
							level->grid.set(yi, xi, player);
							level->playerPos = { xi, yi };
							break;
						}
					case CHU:
						{
							Chu chu {};
							s8 dxi = stoi(strtok(nullptr, ";"));
							s8 dyi = stoi(strtok(nullptr, ";"));
							chu.Dir = { dxi, dyi };
							chu.Func = ChuFuncs[stoi(strtok(nullptr, ";"))];
							level->grid.set(yi, xi, chu);
							break;
						}
					case SCENERY:
						{
							Scenery scenery { .Type = ScenearyType::TREE };
							level->grid.set(yi, xi, scenery);
							break;
						}
					case NEXTLEVELPORTAL:
						{
							NextLevelPortal portal { };
							level->grid.set(yi, xi, portal);
							break;
						}
					default:
						break;
				}
			}
		}

		return level;
	}
}
