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
			return vec2(0.5f, 0.5f);
		} else if (dir == -X_AXIS) {
			return vec2(0.5f, 0.0f);
		} else if (dir == -Y_AXIS) {
			return vec2(0.0f, 0.5f);
		} else {
			return vec2(0.0f, 0.0f);
		}
	}

	void DrawGrid(Grid* grid) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

		vec2 cellScale;
		cellScale.x = 1.0f / grid->Width;
		cellScale.y = 1.0f / grid->Height;

		for (u32 i = 0; i < grid->Width; i++) {
			for (u32 j = 0; j < grid->Height; j++) {
				CellRef* ref = grid->get(i, j);
				if (holds_alternative<Player>(*ref)) {
					Draw(PlayerDrawInfo, getClipPos(grid, i, j), cellScale);
				} else if (holds_alternative<Scenery>(*ref)) {
					Draw(TreeDrawInfo, getClipPos(grid, i, j), cellScale);
				} else if (holds_alternative<Chu>(*ref)) {
					Chu chu = get<Chu>(*ref);
					vec2 texOffset  = texOffsetFromDir(chu.Dir);
					Draw(ChuDrawInfo, getClipPos(grid, i, j), cellScale, texOffset);
				}
			}
		}
	}
}
