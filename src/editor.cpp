#include <core.hpp>
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

	void EditorController(EditorContext& context) {
		if (isKeyPressed(KEY_P)) {
			context.state = EState::PLACING;
		} else if (isKeyPressed(KEY_A)) {
			context.state = EState::ASSIGN_FUNC;
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
	}

	void WriteLevel(string const& path) {

	}

	Level ReadLevel(string const& path) {
		Level level {
			.grid = { 10, 10 }
		};
		level.step = 0;
		level.flags = {};
		level.playerPos = { 0, 0 };

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
							level.grid.set(yi, xi, player);
							level.playerPos = { xi, yi };
							break;
						}
					case CHU:
						{
							Chu chu {};
							u8 dxi = stoi(strtok(nullptr, ";"));
							u8 dyi = stoi(strtok(nullptr, ";"));
							chu.Dir = { dxi, dyi };
							chu.Func = ChuFuncs[stoi(strtok(nullptr, ";"))];
							level.grid.set(yi, xi, chu);
							break;
						}
					case SCENERY:
						{
							Scenery scenery { .Type = ScenearyType::TREE };
							level.grid.set(yi, xi, scenery);
							break;
						}
					case NEXTLEVELPORTAL:
						{
							NextLevelPortal portal { };
							level.grid.set(yi, xi, portal);
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
