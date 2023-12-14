#pragma once

#include <world.hpp>
#include <level.hpp>

namespace game {

	extern void AttackSW(ivec2 pos, Grid* grid, u32 step);

	extern void AttackOrRotate(ivec2 pos, Grid* grid, u32 step);

	extern void CommonRotate(ivec2 const axis[4], ivec2 pos, Grid* grid, u32 step);

	extern ivec2 CommonAttack(ivec2 pos, Grid* grid, u32 step);
};
