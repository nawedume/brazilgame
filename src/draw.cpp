#include <core.hpp>
#include <render.hpp>
#include <world.hpp>
#include <glad/glad.h>
#include <level.hpp>

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
			return vec2(0.0f, 0.50f);
		}
	}

	void DrawEndScreen() {
		DrawBackground(EndScreenDrawInfo);
	}

	void DrawTitleScreen() {
		DrawBackground(TitleScreenDrawInfo);
	}

	void DrawLevel(Level* level) {
		Grid* const grid = &level->grid;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

		DrawBackground(GrassDrawInfo);

		vec2 cellScale;
		cellScale.x = 1.0f / grid->Width;
		cellScale.y = 1.0f / grid->Height;

		for (int i = 0; i < level->rockyGrassPos.size(); i++) {
			ivec2 pos = level->rockyGrassPos[i];
			Draw(RockyGrassDrawInfo, getClipPos(grid, pos.y, pos.x), cellScale);
		}

		for (u32 row = 0; row < grid->Height; row++) {
			for (u32 col = 0; col < grid->Width; col++) {
				CellRef* ref = grid->get(row, col);
				switch (ref->type) {
					case Type::PLAYER:
						{
							vec2 texOffset  = texOffsetFromDir(ref->player.Dir);
							Draw(PlayerDrawInfo, getClipPos(grid, row, col), cellScale, texOffset);
							break;
						}
					case Type::CHU:
						{
							vec2 texOffset  = texOffsetFromDir(ref->chu.Dir);
							Draw(ChuDrawInfo, getClipPos(grid, row, col), cellScale, texOffset);
							break;
						}
					case Type::SCENERY:
						{
							Draw(TreeDrawInfo, getClipPos(grid, row, col), cellScale);
							break;
						}
					case Type::NEXTLEVELPORTAL:
						{
							Draw(NextLevelPortalDrawInfo, getClipPos(grid, row, col), cellScale);
							break;
						}
					case Type::GOAT:
						{
							vec2 texOffset  = texOffsetFromDir(ref->goat.Dir);
							Draw(GoatDrawInfo, getClipPos(grid, row, col), cellScale, texOffset);
							break;
						}

					case Type::WATER:
						{
							Draw(WaterDrawInfo, getClipPos(grid, row, col), cellScale);
							break;
						}
					case Type::MOVEABLE:
						{
							switch (ref->moveable.type) {
								case MoveableType::ROCK:
									{
										Draw(RockDrawInfo, getClipPos(grid, row, col), cellScale);
										break;
									}
								default:
									{
										fprintf(stderr, "No moveable type found\n");
										break;
									}
							}
							break;
						}
					case Type::SACI:
						{
							Draw(SaciDrawInfo, getClipPos(grid, row, col), cellScale);
							break;
						}
					case Type::GRASS:
						{
							break;
						}
				};
			}
		}

		if (level->flags.defeated) {
			game::DrawRestartScreen();
		}
	}
}
