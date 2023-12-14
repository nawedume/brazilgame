#include <stdio.h>
#include <core.hpp>
#include <cstdio>
#include <stdexcept>
#include <glad/glad.h>
#include <window.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>


static void glfwError(int id, const char* desc) {
	printf("%d: %s\n", id, desc);
}

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, 
                            GLsizei length, const char *message, const void *userParam) {

	printf("%d, %d, %d, %d, %s", source, type, id, severity, message);
}

namespace game {
	GLFWwindow* window;

	void setupWindow() {
		glfwInit();

		glfwSetErrorCallback(&glfwError);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef DEBUG 
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

		window = glfwCreateWindow(game::WIDTH, game::HEIGHT, "Build Game", NULL, NULL);
		if (window == nullptr) {
			std::printf("Window cannot be created\n");
			exit(1);
		}

		glfwMakeContextCurrent(window);
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	};

	void resizeWindow() {
		throw std::runtime_error("Not implemented\n");
	}

	bool shouldCloseWindow() {
		if (window == nullptr) {
			printf("Window is null\n");
		}
		return glfwWindowShouldClose(window);
	}

	void closeWindow() {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	void swapBuffers() {
		glfwSwapBuffers(window);
	}

	void windowSystemPollEvents() {
		glfwPollEvents();
	}

	void clearWindow(f32 r, f32 g, f32 b, f32 a) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void setOpaqueRenderingState() {
		glEnable(GL_DEPTH_TEST);
	}

	void setupGlad() {
		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
			printf("Cannot setup glad\n");
			exit(1);
		}

		printf("Using OpenGL version %s\n", glGetString(GL_VERSION));
	}

	void printGraphicErrors() {
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			fprintf(stderr, "OpenGL Error: %d\n", err);
		}
	}
}
