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
		vector<ivec2> rockyGrassPos; // @attention should probably change this to generic "special walkable", this isn't event grass.
	};

	extern Level createLevel(u32 levelIdx);

	extern void next(Level& level);

	extern void processStep(Level& level, ivec2 dir);

	extern vector<ivec2> solveLevel(Level level);
}
