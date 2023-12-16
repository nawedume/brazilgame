#include "world.hpp"
#include <core.hpp>
#include <level.hpp>
#include <input.hpp>
#include <ai.hpp>

namespace game {
	bool isWalkable(CellRef* ref) {
		return ref->type == Type::GRASS || ref->type == Type::NEXTLEVELPORTAL;
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
				level.grid.set(row, col, CellRef(Type::GRASS, grass));
			}
		}

		return level;
	}

	void processStep(Level& level) {
		ivec2 newPlayerPos = getPositionFromEvent(level.playerPos);
		if (newPlayerPos != level.playerPos && isValidMove(newPlayerPos, &level.grid)) {
			CellRef* previousCellRef = level.grid.get(newPlayerPos.y, newPlayerPos.x);
			if (previousCellRef->type == Type::NEXTLEVELPORTAL) {
				level.flags.completed = true;
				return;
			}

			Player player = level.grid.get(level.playerPos.y, level.playerPos.x)->player;
			player.Dir = newPlayerPos - level.playerPos;
			level.grid.remove(level.playerPos.y, level.playerPos.x);
			level.grid.set(newPlayerPos.y, newPlayerPos.x, CellRef(Type::PLAYER, player));
			level.playerPos = newPlayerPos;
			level.step += 1;

			Grid newGrid = level.grid;
			for (u32 row = 0; row < level.grid.Height; row++) {
				for (u32 col = 0; col < level.grid.Width; col++) {
					game::CellRef* ref = level.grid.get(row, col);
					if (ref->type == Type::CHU) {
						game::Chu chu = ref->chu;
						chu.Func({ col, row }, &newGrid, level.step);
					}
				}
			}
			level.grid = newGrid;
		}
	}

	void next(Level& level) {
		processStep(level);
	}
}
