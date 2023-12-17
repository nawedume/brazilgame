#include "world.hpp"
#include <core.hpp>
#include <level.hpp>
#include <input.hpp>
#include <ai.hpp>

namespace game {
	bool isWalkable(CellRef* ref) {
		return ref->type == Type::GRASS || ref->type == Type::NEXTLEVELPORTAL;
	}

	bool isValidMove(ivec2 position, Grid* grid) {
		game::CellRef* ref = grid->get(position.y, position.x); 
		return game::isInBounds(position, grid) && (isWalkable(ref) || ref->type == Type::MOVEABLE);
	}

	bool isPushableArea(CellRef* ref) {
		return ref->type == Type::GRASS || ref->type == Type::WATER;
	}

	bool isValidMoveableMove(ivec2 position, Grid* grid) {
		game::CellRef* ref = grid->get(position.y, position.x); 
		return game::isInBounds(position, grid) && isPushableArea(ref);
	}

	ivec2 getDirectionFromEvent() {
			if (game::isKeyClicked(game::KEY_D)) {
				return X_AXIS;
			}
			else if (game::isKeyClicked(game::KEY_A)) {
				return -X_AXIS;
			}
			else if (game::isKeyClicked(game::KEY_W)) {
				return Y_AXIS;
			}
			else if (game::isKeyClicked(game::KEY_S)) {
				return -Y_AXIS;
			}
			return ivec2(0, 0);
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
		ivec2 dir = getDirectionFromEvent();
		ivec2 newPlayerPos = level.playerPos + dir;
		if (dir != ivec2(0, 0) && isValidMove(newPlayerPos, &level.grid)) {
			CellRef* previousCellRef = level.grid.get(newPlayerPos.y, newPlayerPos.x);
			if (previousCellRef->type == Type::NEXTLEVELPORTAL) {
				level.flags.completed = true;
				return;
			} else if (previousCellRef->type == Type::MOVEABLE) {
				ivec2 newMoveablePos = newPlayerPos + dir;
				if (!isValidMoveableMove(newMoveablePos, &level.grid)) {
					return;
				} else {
					CellRef* pMoveableCellRef= level.grid.get(newMoveablePos);
					if (Type::WATER == pMoveableCellRef->type) {
						level.grid.remove(newPlayerPos);
						level.grid.set(newMoveablePos, { GRASS, Grass { } });
						level.rockyGrassPos.push_back(newMoveablePos);
					} else {
						level.grid.move(newPlayerPos, newMoveablePos);
					}
				}
			}

			Player player = level.grid.get(level.playerPos.y, level.playerPos.x)->player;
			player.Dir = newPlayerPos - level.playerPos;
			ivec2 oldPlayerPos = level.playerPos;
			level.grid.move(oldPlayerPos, newPlayerPos);
			level.playerPos = newPlayerPos;
			level.step += 1;

			if (!level.goatPos.empty()) {
				ivec2 nextPos = oldPlayerPos;
				for (u32 i = 0; i < level.goatPos.size(); i++) {
					ivec2 oldPos = level.goatPos[i];
					level.goatPos[i] = nextPos;
					level.grid.move(oldPos, nextPos);
					nextPos = oldPos;
				}
			}

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
			level.flags.defeated = level.grid.get(level.playerPos.y, level.playerPos.x)->type != Type::PLAYER;
			if (!level.flags.defeated) {
				for (ivec2& gpos : level.goatPos) {
					if (level.grid.get(gpos)->type != Type::GOAT) {
						level.flags.defeated = true;
						break;
					}
				}
			}
		}
	}

	void next(Level& level) {
		processStep(level);
	}
}
