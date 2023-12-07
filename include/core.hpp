#pragma once

#include <cinttypes>
#include <vector>
#include <glm/glm.hpp>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;

typedef float f32;
typedef double f64;

using namespace glm;

namespace game {
	using namespace std;
	using namespace glm;

	struct Block {
		vec2 WorldPosStart {};
		vec2 WorldPosEnd {};
	};

	struct World {
		vector<Block> StaticBlocks;
		vector<Block> DynamicBlocks;
	};

}
