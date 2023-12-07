#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "core.hpp"
#include "settings.hpp"
#include <GLFW/glfw3.h>

namespace game {

	extern GLFWwindow* window;

	extern void setupWindow();

	extern void resizeWindow();

	extern bool shouldCloseWindow();

	extern void closeWindow();

	extern void windowSystemPollEvents();
	
	extern void swapBuffers();

	extern void clearWindow(f32 r, f32 g, f32 b, f32 a);

	extern void setOpaqueRenderingState();

	extern void setupGlad();

	extern void printGraphicErrors();
}

#endif
