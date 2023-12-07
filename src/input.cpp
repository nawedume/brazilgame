#include <core.hpp>
#include <input.hpp>
#include <window.hpp>
#include <GLFW/glfw3.h>

#define NUM_KEYS 512

namespace game {

	const u32 KEY_E = GLFW_KEY_E;
	const u32 KEY_Q = GLFW_KEY_Q;
	const u32 KEY_W = GLFW_KEY_W;
	const u32 KEY_A = GLFW_KEY_A;
	const u32 KEY_S = GLFW_KEY_S;
	const u32 KEY_D = GLFW_KEY_D;
	const u32 KEY_SPACE = GLFW_KEY_SPACE;
	const u32 KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;

	MM MouseMovement = {};

	u32 clickMap[NUM_KEYS] {};
	u32 currentFrame = 0;

	bool isKeyClicked(int key) {
		bool isClicked = glfwGetKey(window, key) == GLFW_PRESS;
		if (isClicked && clickMap[key] < currentFrame) {
			clickMap[key] = currentFrame + REFRESH_RATE;
			return true;
		}

		return false;
	}

	bool isKeyPressed(int key) {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

	bool isFirstMouseMove = true;
	void updateMouseMovement() {
		f64 xpos;
		f64 ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		if (isFirstMouseMove) {
			MouseMovement.xPos = xpos;
			MouseMovement.yPos = ypos;
			isFirstMouseMove = false;
		}

		MouseMovement.xOffset = xpos - MouseMovement.xPos;
		MouseMovement.yOffset = ypos - MouseMovement.yPos;

		MouseMovement.xPos = xpos;
		MouseMovement.yPos = ypos;
	}

	void tick() {
		currentFrame += 1;
		updateMouseMovement();
	}
}