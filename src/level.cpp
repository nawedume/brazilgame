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

	bool isValidSaciMove(ivec2 position, Grid* grid) {
		game::CellRef* ref = grid->get(position.y, position.x); 
		return isValidMove(position, grid); //&& (ref->type != Type::GRASS || ref->grass.type != GrassType::ROCKY);
	}

	bool isPushableArea(CellRef* ref) {
		return ref->type == Type::GRASS || ref->type == Type::WATER;
	}

	bool isValidMoveableMove(ivec2 position, Grid* grid) {
		game::CellRef* ref = grid->get(position.y, position.x); 
		return game::isInBounds(position, grid) && isPushableArea(ref);
	}

	ivec2 getDirectionFromEvent() {
			if (game::isKeyClicked(game::KEY_D) || isKeyClicked(KEY_RIGHT)) {
				return X_AXIS;
			}
			else if (game::isKeyClicked(game::KEY_A) || isKeyClicked(KEY_LEFT)) {
				return -X_AXIS;
			}
			else if (game::isKeyClicked(game::KEY_W) || isKeyClicked(KEY_UP)) {
				return Y_AXIS;
			}
			else if (game::isKeyClicked(game::KEY_S) || isKeyClicked(KEY_DOWN)) {
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

	bool processMoveableMove(ivec2 newPlayerPos, ivec2 dir, Level& level) {
		ivec2 newMoveablePos = newPlayerPos + dir;
		if (!isValidMoveableMove(newMoveablePos, &level.grid)) {
			return false;
		} else {
			CellRef* pMoveableCellRef= level.grid.get(newMoveablePos);
			if (Type::WATER == pMoveableCellRef->type) {
				level.grid.remove(newPlayerPos);
				level.grid.set(newMoveablePos, { GRASS, Grass { .type = GrassType::ROCKY } });
				level.rockyGrassPos.push_back(newMoveablePos);
			} else {
				level.grid.move(newPlayerPos, newMoveablePos);
			}
			return true;
		}
	}


	void processStep(Level& level, ivec2 dir) {
		ivec2 newPlayerPos = level.playerPos + dir;
		if (dir != ivec2(0, 0) && isValidMove(newPlayerPos, &level.grid)) {
			CellRef* previousCellRef = level.grid.get(newPlayerPos.y, newPlayerPos.x);
			if (previousCellRef->type == Type::NEXTLEVELPORTAL) {
				level.flags.completed = true;
				return;
			} else if (previousCellRef->type == Type::MOVEABLE) {
				if (!processMoveableMove(newPlayerPos, dir, level)) {
					return;
				}
			}

			Player player = level.grid.get(level.playerPos.y, level.playerPos.x)->player;
			//player.Dir = newPlayerPos - level.playerPos;
			ivec2 oldPlayerPos = level.playerPos;
			level.grid.move(oldPlayerPos, newPlayerPos);
			level.playerPos = newPlayerPos;
			level.grid.get(level.playerPos)->player.Dir = dir;
			level.step += 1;

			if (!level.goatPos.empty()) {
				ivec2 nextPos = oldPlayerPos;
				ivec2 nextDir = dir; 
				for (u32 i = 0; i < level.goatPos.size(); i++) {
					ivec2 oldPos = level.goatPos[i];
					ivec2 oldDir = level.grid.get(oldPos)->goat.Dir;
					level.goatPos[i] = nextPos;
					level.grid.move(oldPos, nextPos);
					level.grid.get(nextPos)->goat.Dir = nextDir;
					nextDir = oldDir;
					nextPos = oldPos;
				}
			}

			if (!level.saciPos.empty()) {
				for (int i = 0; i < level.saciPos.size(); i++) {
					ivec2 saciPos = level.saciPos[i];
					CellRef* ref = level.grid.get(saciPos);
					assert(ref->type == Type::SACI);
					switch (ref->saci.moveType) {
						case SaciMoveType::VCOPY:
							{
								ivec2 newSaciPos = saciPos + ivec2(0, dir.y);
								if (isValidMove(newSaciPos, &level.grid) && oldPlayerPos.y == saciPos.y) {
									level.grid.move(saciPos, newSaciPos);
									level.saciPos[i] = newSaciPos;
								}
								break;
							}
						case SaciMoveType::INVERSE_MIMIC:
							{
								ivec2 newSaciPos = saciPos - dir;
								if (isValidMove(newSaciPos, &level.grid)) {
									CellRef* previousCellRef = level.grid.get(newSaciPos);
									if (previousCellRef->type == Type::MOVEABLE) {
										if (!processMoveableMove(newSaciPos, -dir, level)) {
											return;
										}
									}

									level.grid.move(saciPos, newSaciPos);
									level.saciPos[i] = newSaciPos;
								}
								break;
							}
						default:
							{
								throw std::runtime_error("Saci move type not there\n");
							}
					}
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

				vector<ivec2> newSPos;
				for (ivec2& spos : level.saciPos) {
					if (level.grid.get(spos)-> type == Type::SACI) {
						newSPos.push_back(spos);
					}
					level.saciPos = newSPos;
				}
			}
		}
	}

	void next(Level& level) {
		ivec2 dir = getDirectionFromEvent();
		processStep(level, dir);
	}

	bool solve(Level level, vector<ivec2>& instructions) {
		if (level.step != instructions.size()) {
			printf("%d, %lu\n", level.step, instructions.size());
		}

		if (level.step > 25) {
			return false;
		}

		ivec2 Dirs[4] = { X_AXIS, Y_AXIS, -X_AXIS, -Y_AXIS };
		Level pLevel = level;

		for (auto dir : Dirs) {
			level = pLevel;
			processStep(level, dir);

			if (level.flags.defeated) {
				continue;
			}
			else {
				instructions.push_back(dir);

				if (level.flags.completed) {
					return true;
				}
				else if (level.step != pLevel.step) {
					if (solve(level, instructions)) {
						return true;
					} else {
						instructions.pop_back();
					}
				} else {
					instructions.pop_back();
				}
			}
		}

		return false;
	}

	vector<ivec2> solveLevel(Level level) {
		vector<ivec2> instructions {}; 
		if (!solve(level, instructions)) {
			printf("Not solved\n");
		} else {
			printf("Solved\n");
		}

		//for (auto inst : instructions) {
		//	printf("%d, %d\n", inst.x, inst.y);
		//}

		return instructions;
	}
}
