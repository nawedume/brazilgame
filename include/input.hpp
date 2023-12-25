#ifndef INPUT_HPP
#define INPUT_HPP

#include <core.hpp>
#include <GLFW/glfw3.h>

namespace game {

	const f64 REFRESH_RATE = 0.2;
	extern const u32 KEY_E;
	extern const u32 KEY_Q;
	extern const u32 KEY_W;
	extern const u32 KEY_A;
	extern const u32 KEY_S;
	extern const u32 KEY_D;
	extern const u32 KEY_R;
	extern const u32 KEY_P;
	extern const u32 KEY_O;
	extern const u32 KEY_UP;
	extern const u32 KEY_DOWN;
	extern const u32 KEY_RIGHT;
	extern const u32 KEY_LEFT;
	extern const u32 KEY_SPACE;
	extern const u32 KEY_LEFT_SHIFT;

	struct MM {
		f32 xOffset {};
		f32 yOffset {};
		f32 xPos {};
		f32 yPos {};
	};

	extern MM MouseMovement;

	extern bool isKeyClicked(int key);

	extern bool isKeyPressed(int key);
	
	extern s8 getNumericKeyPressed(); 

	extern bool isLeftMouseClicked(vec2* pos);

	extern bool isRightMouseClicked(vec2* pos);

	extern void tick();
}

#endif
