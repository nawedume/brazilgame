#include <core.hpp>
#include <level.hpp>
#include <input.hpp>
#include <stdexcept>

namespace game {

	bool isValidMove(ivec2 position, game::Grid* grid) {
		game::CellRef* ref = grid->get(position.y, position.x); 
		return game::isInBounds(position, grid) && holds_alternative<game::Grass>(*ref);
	}

	ivec2 getPositionFromEvent(ivec2 oldPos) {
			ivec2 newPlayerPos = oldPos;
			if (game::isKeyClicked(game::KEY_D)) {
				newPlayerPos += X_AXIS;
			}
			else if (game::isKeyClicked(game::KEY_A)) {
				newPlayerPos -= X_AXIS;
			}
			else if (game::isKeyClicked(game::KEY_W)) {
				newPlayerPos += Y_AXIS;
			}
			else if (game::isKeyClicked(game::KEY_S)) {
				newPlayerPos -= Y_AXIS;
			}
			return newPlayerPos;
	}

	Level createLevel1() {
		Level level {
			.grid = Grid(10, 10),
			.player = { .Dir = X_AXIS },
			.playerPos = { 4, 6 },
			.step = 0
		};

		level.grid.set(level.playerPos.y, level.playerPos.x, level.player);

		game::Scenery scenery = { };
		level.grid.set(0, 3, scenery);
		level.grid.set(2, 3, scenery);
		level.grid.set(3, 3, scenery);

		game::ControllerFn fn = [](ivec2 pos, game::Grid* grid, u32 step) {
			ivec2 const rotations[4] = { -X_AXIS, -Y_AXIS, -X_AXIS, -Y_AXIS};
			ivec2 newPos = game::CommonAttack(pos, grid, step);

			if (pos == newPos) {
				game::CommonRotate(rotations, newPos, grid, step);
			}
		};
		game::Chu chu = { .Dir = -X_AXIS, .Func = fn };
		level.grid.set(3, 2, chu);
		return level;
	}

	Level createLevel2() {
		Level level {
			.grid = Grid(10, 10),
			.player = { .Dir = X_AXIS },
			.playerPos = { 6, 3 },
			.step = 0
		};

		level.grid.set(level.playerPos.y, level.playerPos.x, level.player);

		game::Scenery scenery = { };
		level.grid.fillCol(0, scenery);
		level.grid.fillCol(1, scenery);
		level.grid.fillCol(2, scenery);
		level.grid.fillCol(3, scenery);
		level.grid.fillCol(4, scenery);
		level.grid.fillCol(5, scenery);
		level.grid.fillRow(0, scenery);
		level.grid.fillRow(1, scenery);
		level.grid.fillRow(2, scenery);
		level.grid.fillRow(9, scenery);
		level.grid.fillRow(8, scenery);
		level.grid.fillRow(7, scenery);

		game::ControllerFn fn = [](ivec2 pos, game::Grid* grid, u32 step) {
			ivec2 const rotations[4] = { -X_AXIS, -Y_AXIS, -X_AXIS, -Y_AXIS};
			ivec2 newPos = game::CommonAttack(pos, grid, step);

			if (pos == newPos) {
				game::CommonRotate(rotations, newPos, grid, step);
			}
		};
		game::Chu chu = { .Dir = -X_AXIS, .Func = fn };
		level.grid.set(6, 9, chu);
		return level;
	}

	Level createLevel3() {
		Level level = {
			.grid = Grid (10, 10),
			.player = { .Dir = X_AXIS },
			.playerPos = { 4, 4 },
			.step = 0
		};

		level.grid.set(level.playerPos.y, level.playerPos.x, level.player);
		game::Scenery scenery = { };
		level.grid.fillRow(0, scenery);
		level.grid.fillRow(1, scenery);
		level.grid.fillRow(2, scenery);

		level.grid.fillRow(9, scenery);
		level.grid.fillRow(8, scenery);
		level.grid.fillRow(7, scenery);

		level.grid.fillCol(7, scenery);
		level.grid.fillCol(8, scenery);
		level.grid.fillCol(9, scenery);

		return level;

	}

	void next(Level& level) {
		ivec2 newPlayerPos = getPositionFromEvent(level.playerPos);
		if (newPlayerPos != level.playerPos && isValidMove(newPlayerPos, &level.grid)) {
			level.grid.remove(level.playerPos.y, level.playerPos.x);
			level.grid.set(newPlayerPos.y, newPlayerPos.x, level.player);
			level.playerPos = newPlayerPos;
			level.step += 1;

			for (u32 row = 0; row < level.grid.Height; row++) {
				for (u32 col = 0; col < level.grid.Width; col++) {
					game::CellRef* ref = level.grid.get(row, col);
					if (holds_alternative<game::Chu>(*ref)) {
						game::Chu chu = get<game::Chu>(*ref);
						chu.Func({ col, row }, &level.grid, level.step);
					}
				}
			}
		}
	}

	Level createLevel(u32 levelIdx) {
		switch (levelIdx) {
			case 1: return createLevel1();
			case 2: return createLevel2();
			case 3: return createLevel3();
			default: throw std::runtime_error("No level found\n");
		}
	}
}
