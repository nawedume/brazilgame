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
#include <editor.hpp>

using namespace glm;
using namespace std;

int main() {
	game::setupWindow();
	game::setupGlad();
	game::InitRenderState();

	u32 levelIdx = 0;
	game::Level ml = game::ReadLevel("./levels/1.lvl");
	game::Level currentLevel = game::createLevel(2);;

	f32 PreviousTime = 0.0f;
	bool isEditor = false;
	game::EditorContext editorContext {};
	editorContext.grid = &currentLevel.grid;
	editorContext.entId = static_cast<game::EntId>(0);
	editorContext.state = game::EState::PLACING;
	editorContext.chuFunc = 0;

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
			if (isEditor) {
				currentLevel = game::createLevel(0);
			} else {
				currentLevel = game::createLevel(levelIdx);
			}
		}

		if (game::isKeyClicked(game::KEY_E)) {
			isEditor = !isEditor;
			printf("Editor mode: %b\n", isEditor);
		}

		if (currentLevel.flags.completed) {
			levelIdx += 1;
			currentLevel = game::createLevel(levelIdx);
		}

		if (isEditor) {
			game::EditorController(editorContext);
		} else {
			game::next(currentLevel);
		}

		editorContext.grid = &currentLevel.grid;
		game::DrawGrid(&currentLevel.grid);

		game::swapBuffers();
		game::windowSystemPollEvents();
		game::tick();
	}

	return 0;
}
