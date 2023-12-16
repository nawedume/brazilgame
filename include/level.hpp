#pragma once

#include <core.hpp>
#include <world.hpp>
#include <string>

namespace game {
	using namespace std;

	struct Flags {
		u8 completed : 1;
	};

	struct Level {
		Grid grid;
		ivec2 playerPos;
		u32 step;
		Flags flags { 0 };
		string levelName;
	};

	extern Level createLevel(u32 levelIdx);

	extern void next(Level& level);
}
