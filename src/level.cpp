#include "world.hpp"
#include <core.hpp>
#include <level.hpp>
#include <input.hpp>
#include <stdexcept>
#include <variant>
#include <ai.hpp>

namespace game {
	bool isWalkable(CellRef* ref) {
		return holds_alternative<Grass>(*ref) || holds_alternative<NextLevelPortal>(*ref);
	}

	bool isValidMove(ivec2 position, game::Grid* grid) {
		game::CellRef* ref = grid->get(position.y, position.x); 
		return game::isInBounds(position, grid) && isWalkable(ref);
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

	Level createEmptyLevel() {
		Level level {
			.grid = Grid(10, 10),
			.step = 0
		};

		for (int row = 0; row < level.grid.Height; row++) {
			for (int col = 0; col < level.grid.Width; col++) {
				Grass grass = {};
				level.grid.set(row, col, grass);
			}
		}

		return level;
	}

	Level createLevel1() {
		Level level {
			.grid = Grid(10, 10),
			.playerPos = { 4, 6 },
			.step = 0,
		};

		Player player { .Dir = X_AXIS };
		level.grid.set(level.playerPos.y, level.playerPos.x, player);

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

		NextLevelPortal endPortal {};
		level.grid.set(5, 5, endPortal);
		return level;
	}

	Level createLevel2() {
		Level level {
			.grid = Grid(10, 10),
			.playerPos = { 4, 4 },
			.step = 0,
		};

		Player player { .Dir = X_AXIS };
		level.grid.set(level.playerPos.y, level.playerPos.x, player);

		game::Scenery scenery = { };
		level.grid.fillCol(0, scenery);
		level.grid.fillCol(1, scenery);
		level.grid.fillCol(2, scenery);
		level.grid.fillCol(3, scenery);

		level.grid.fillRow(0, scenery);
		level.grid.fillRow(1, scenery);
		level.grid.fillRow(2, scenery);
		level.grid.fillRow(3, scenery);

		level.grid.fillRow(9, scenery);
		level.grid.fillRow(8, scenery);
		level.grid.fillRow(7, scenery);

		level.grid.set(5, 5, scenery);
		level.grid.set(4, 7, scenery);
		level.grid.set(4, 8, scenery);
		level.grid.set(4, 9, scenery);

		level.grid.set(6, 7, scenery);
		level.grid.set(6, 8, scenery);
		level.grid.set(6, 9, scenery);


		game::ControllerFn fn = [](ivec2 pos, game::Grid* grid, u32 step) {
			ivec2 const rotations[4] = { -X_AXIS, -Y_AXIS, -X_AXIS, -Y_AXIS};
			ivec2 newPos = game::CommonAttack(pos, grid, step);

			if (pos == newPos) {
				game::CommonRotate(rotations, newPos, grid, step);
			}
		};
		game::Chu chu = { .Dir = -Y_AXIS, .Func = fn };
		level.grid.set(6, 6, chu);

		NextLevelPortal endPortal {};
		level.grid.set(5, 9, endPortal);
		return level;
	}

	Level createLevel3() {
		Level level = {
			.grid = Grid (10, 10),
			.playerPos = { 4, 4 },
			.step = 0
		};

		Player player = { .Dir = X_AXIS };
		level.grid.set(level.playerPos.y, level.playerPos.x, player);
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

		NextLevelPortal endPortal {};
		level.grid.set(10, 10, endPortal);

		return level;

	}

	void processStep(Level& level) {
		ivec2 newPlayerPos = getPositionFromEvent(level.playerPos);
		if (newPlayerPos != level.playerPos && isValidMove(newPlayerPos, &level.grid)) {
			CellRef* o = level.grid.get(6, 9);
			CellRef* previousCellRef = level.grid.get(newPlayerPos.y, newPlayerPos.x);
			if (holds_alternative<NextLevelPortal>(*previousCellRef)) {
				level.flags.completed = true;
				return;
			}

			Player player = get<Player>(*level.grid.get(level.playerPos.y, level.playerPos.x));
			player.Dir = newPlayerPos - level.playerPos;
			level.grid.remove(level.playerPos.y, level.playerPos.x);
			level.grid.set(newPlayerPos.y, newPlayerPos.x, player);
			level.playerPos = newPlayerPos;
			level.step += 1;

			Grid newGrid = level.grid;
			for (u32 row = 0; row < level.grid.Height; row++) {
				for (u32 col = 0; col < level.grid.Width; col++) {
					game::CellRef* ref = level.grid.get(row, col);
					if (holds_alternative<game::Chu>(*ref)) {
						game::Chu chu = get<game::Chu>(*ref);
						chu.Func({ col, row }, &newGrid, level.step);
					}
				}
			}
			level.grid = newGrid;
		}
	}

	Level createLevel(u32 levelIdx) {
		switch (levelIdx) {
			case 0: return createEmptyLevel();
			case 1: return createLevel1();
			case 2: return createLevel2();
			case 3: return createLevel3();
			default: throw std::runtime_error("No level found\n");
		}
	}

	void next(Level& level) {
		processStep(level);
	}
}
