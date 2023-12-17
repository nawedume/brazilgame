#include <ai.hpp>
#include <level.hpp>

namespace game {

	bool hasBlock(ivec2 position, game::Grid* grid) {
		CellRef* ref = grid->get(position.y, position.x);
		return ref->type != GRASS;
	}

	bool isInBounds(ivec2 position, game::Grid* grid) {
		return position.x >= 0 && position.x < grid->Width
			&& position.y >= 0 && position.y < grid->Height; }

	bool hasKillable(ivec2 position, Grid* grid) {
		CellRef* ref = grid->get(position.y, position.x);
		return ref->type == Type::PLAYER || ref->type == Type::GOAT || ref->type == Type::SACI;
	}

	void CommonRotate(ivec2 const axis[4], ivec2 pos, Grid* grid, u32 step) {
		CellRef* ref = (grid->get(pos.y, pos.x));
		if (ref->type == Type::CHU) {
			Chu* chu = &ref->chu;
			for (int i = 0; i < 4; i++) {
				if (axis[i] == chu->Dir) {
					chu->Dir = axis[(i + 1) % 4];
					return;
				}
			}

			chu->Dir = axis[0];
		}
	}

	ivec2 CommonAttack(ivec2 pos, Grid* grid, u32 step) {
		CellRef* ref = grid->get(pos.y, pos.x);

		if (ref->type == Type::CHU) {
			Chu chu = ref->chu;
			vec2 curCell = pos + chu.Dir;
			while (isInBounds(curCell, grid)) {

				if (hasKillable(curCell, grid)) {
					ivec2 newPos = pos + chu.Dir;
					grid->set(newPos.y, newPos.x, *ref);
					grid->remove(pos.y, pos.x);
					return newPos;
				} else if (hasBlock(curCell, grid)) {
						return pos;
				}	

				curCell += chu.Dir;
			}
		}

		return pos;
	};

	void AttackSW(ivec2 pos, Grid* grid, u32 step) {
		ivec2 const rotations[4] = { -X_AXIS, -Y_AXIS, -X_AXIS, -Y_AXIS};

		ivec2 newPos = game::CommonAttack(pos, grid, step);
		if (pos == newPos) {
			game::CommonRotate(rotations, newPos, grid, step);
		}
	}

	ivec2 rotateAxis(ivec2 axis) {
		return { -axis.y, axis.x }; 
	}

	void AttackOrRotate(ivec2 pos, Grid* grid, u32 step) {
		ivec2 newPos = game::CommonAttack(pos, grid, step);
		if (pos == newPos) {
			Chu& ref = grid->get(pos.y, pos.x)->chu;
			ivec2 newDir = rotateAxis(ref.Dir);
			while (newDir != ref.Dir) {
				ivec2 neighbourPos = pos + newDir;
				Type neighbourType = grid->get(neighbourPos.y, neighbourPos.x)->type;
				// @attention moveable should just be scenery
				if (neighbourType != Type::SCENERY && neighbourType != Type::MOVEABLE) {
					ref.Dir = newDir;
					return;
				}
				newDir = rotateAxis(newDir);
			}
		}
	}
};
