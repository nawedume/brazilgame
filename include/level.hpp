#pragma once

#include <core.hpp>
#include <world.hpp>
#include <string>

namespace game {
	using namespace std;

	struct Flags {
		u8 completed : 1;
		u8 defeated: 1;
	};

	struct Level {
		Grid grid;
		u32 step;
		ivec2 playerPos;
		Flags flags { 0 };
		string levelName;
		vector<ivec2> goatPos;
	};

	extern Level createLevel(u32 levelIdx);

	extern void next(Level& level);
}
