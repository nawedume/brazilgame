#pragma once

#include <core.hpp>
#include <world.hpp>

namespace game {
	using namespace std;

	struct Level {
		Grid grid;
		Player player;
		ivec2 playerPos;
		u32 step;
	};

	extern Level createLevel(u32 levelIdx);

	extern void next(Level& level);
};
