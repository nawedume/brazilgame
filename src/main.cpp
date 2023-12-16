#include "world.hpp"
#include <core.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

string levelToName(u8 lvl) {
	char b[] = "levels/a.lvl";
	b[7] = '0' + lvl;
	return string(b);
}

int main() {
	game::setupWindow();
	game::setupGlad();
	game::InitRenderState();

	u32 levelIdx = 1;
	game::Level* currentLevel = game::ReadLevel(levelToName(levelIdx));
	//game::Level currentLevel = game::createLevel(2);;

	f32 PreviousTime = 0.0f;
	bool isEditor = false;
	game::EditorContext editorContext {};
	editorContext.level = currentLevel;
	editorContext.entId = static_cast<game::Type>(2);
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
				currentLevel = game::ReadLevel(currentLevel->levelName);
			} else {
				currentLevel = game::ReadLevel(currentLevel->levelName);
			}
		}

		if (game::isKeyClicked(game::KEY_E)) {
			isEditor = !isEditor;
			printf("Editor mode: %d\n", isEditor);
		}

		if (currentLevel->flags.completed) {
			levelIdx += 1;
			currentLevel = game::ReadLevel(levelToName(levelIdx));
		}

		if (isEditor) {
			game::EditorController(editorContext);
			if (editorContext.level != currentLevel) {
				currentLevel = editorContext.level;
			}
		} else {
			game::next(*currentLevel);
		}

		if (currentLevel->grid.get(currentLevel->playerPos.y, currentLevel->playerPos.x)->type != game::Type::PLAYER) {
			// player has died
			printf("Player has died\n");
			currentLevel = game::ReadLevel(currentLevel->levelName);
		}

		editorContext.level = currentLevel;
		game::DrawGrid(&currentLevel->grid);

		game::swapBuffers();
		game::windowSystemPollEvents();
		game::tick();
	}

	return 0;
}
