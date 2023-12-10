#pragma once

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

const ivec2 X_AXIS = vec2(1.0, 0.0);
const ivec2 Y_AXIS = vec2(0.0, 1.0);

namespace game {
	using namespace std;
	using namespace glm;

	struct Block {
		vec2 WorldPosStart {};
		vec2 WorldPosEnd {};
	};

}
