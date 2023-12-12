#include "world.hpp"
#include <core.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <variant>
#include <window.hpp>
#include <input.hpp>
#include <glm/glm.hpp>
#include <shader.hpp>
#include <render.hpp>
#include <core.hpp>
#include <draw.hpp>
#include <world.hpp>
#include <level.hpp>

using namespace glm;
using namespace std;

int main() {
	game::setupWindow();
	game::setupGlad();
	game::InitRenderState();

	u32 levelIdx = 2;
	game::Level currentLevel = game::createLevel(levelIdx);

	f32 PreviousTime = 0.0f;
	while (!game::shouldCloseWindow()) {
#ifdef DEBUG
	if (game::isKeyClicked(game::KEY_Q)) {
		game::closeWindow();
	}
#endif
		f32 CurrentTime = glfwGetTime(); 
		f32 DeltaTime = CurrentTime - PreviousTime;
		PreviousTime = CurrentTime;
		game::clearWindow(0.6, 0.8, 0.6, 1.0);

		if (game::isKeyClicked(game::KEY_R)) {
			currentLevel = game::createLevel(levelIdx);
		}

		game::next(currentLevel);
		game::DrawGrid(&currentLevel.grid);

		game::swapBuffers();
		game::windowSystemPollEvents();
		game::tick();
	}

	return 0;
}
