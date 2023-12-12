#include <core.hpp>
#include <render.hpp>
#include <world.hpp>
#include <variant>
#include <glad/glad.h>

namespace game {

	inline
	vec2 getClipPos(Grid* grid, u8 i, u8 j) {
		f32 y = static_cast<float>(i) / grid->Height;
		f32 x = static_cast<float>(j) / grid->Width;

		return { x, y };
	}

	vec2 texOffsetFromDir(ivec2 dir) {
		if (dir == X_AXIS) {
			return vec2(0.0f, 0.25f);
		} else if (dir == -X_AXIS) {
			return vec2(0.0f, 0.0f);
		} else if (dir == -Y_AXIS) {
			return vec2(0.0f, 0.75f);
		} else {
			return vec2(0.0f, 0.55f);
		}
	}

	void DrawGrid(Grid* grid) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

		vec2 cellScale;
		cellScale.x = 1.0f / grid->Width;
		cellScale.y = 1.0f / grid->Height;

		for (u32 row = 0; row < grid->Height; row++) {
			for (u32 col = 0; col < grid->Width; col++) {
				CellRef* ref = grid->get(row, col);
				if (holds_alternative<Player>(*ref)) {
					Draw(PlayerDrawInfo, getClipPos(grid, row, col), cellScale);
				} else if (holds_alternative<Scenery>(*ref)) {
					Draw(TreeDrawInfo, getClipPos(grid, row, col), cellScale);
				} else if (holds_alternative<Chu>(*ref)) {
					Chu chu = get<Chu>(*ref);
					vec2 texOffset  = texOffsetFromDir(chu.Dir);
					Draw(ChuDrawInfo, getClipPos(grid, row, col), cellScale * 0.8f, texOffset);
				}
			}
		}
	}
}
