#include <core.hpp>
#include <variant>
#include <world.hpp>

namespace game {

	bool hasBlock(ivec2 position, game::Grid* grid) {
		CellRef ref = *(grid->get(position.y, position.x));
		return !holds_alternative<Grass>(ref);
	}

	bool isInBounds(ivec2 position, game::Grid* grid) {
		return position.x >= 0 && position.x < grid->Width
			&& position.y >= 0 && position.y < grid->Height; }

	bool hasKillable(ivec2 position, Grid* grid) {
		CellRef ref = *(grid->get(position.y, position.x));
		return holds_alternative<Player>(ref);
	}

	void CommonRotate(ivec2 const axis[4], ivec2 pos, Grid* grid, u32 step) {
		CellRef* ref = (grid->get(pos.y, pos.x));
		if (holds_alternative<Chu>(*ref)) {
			Chu* chu = &(get<Chu>(*ref));
			for (int i = 0; i < 4; i++) {
				if (axis[i] == chu->Dir) {
					chu->Dir = axis[(i + 1) % 4];
					return;
				}
			}
		}
	}

	ivec2 CommonAttack(ivec2 pos, Grid* grid, u32 step) {
		CellRef ref = *(grid->get(pos.y, pos.x));

		if (holds_alternative<Chu>(ref)) {
			Chu chu = get<Chu>(ref);

			vec2 curCell = pos + chu.Dir;
			while (isInBounds(curCell, grid)) {

				if (hasKillable(curCell, grid)) {
					grid->remove(pos.y, pos.x);
					ivec2 newPos = pos + chu.Dir;
					grid->set(newPos.y, newPos.x, chu);
					return newPos;
				} else if (hasBlock(curCell, grid)) {
						return pos;
				}	

				curCell += chu.Dir;
			}
		}

		return pos;
	};
}
