#include <iostream>
#include <core.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <window.hpp>
#include <input.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <shader.hpp>
#include <render.hpp>

using namespace glm;
using namespace std;

int main() {
	game::setupWindow();
	game::setupGlad();

	game::InitRenderState();
	Shader* BlockShader = new Shader("./shaders/block.vs", "./shaders/block.fs");
	game::RenderAddBlock(game::StaticRenderState, { { 0.0, 0.0 }, { 32.0, 3.0 } }, { 0.4, 0.1, 0.1 });
	game::RenderAddBlock(game::StaticRenderState, { { 0.0, 32.0 }, { 32.0, 32.0 - 3.0 } }, { 0.4, 0.1, 0.1 });

	game::RenderAddBlock(game::StaticRenderState, { { 0.0, 3.0 }, { 3.0, 32.0 - 3.0 } }, { 0.4, 0.1, 0.1 });

	while (!game::shouldCloseWindow()) {
#ifdef DEBUG
	if (game::isKeyClicked(game::KEY_Q)) {
		game::closeWindow();
	}
#endif
		game::clearWindow(0.1, 0.3, 0.1, 1.0);
		if (game::isKeyPressed(game::KEY_W)) {
			game::camera->Pos.y += 0.01f;
		}
		if (game::isKeyPressed(game::KEY_S)) {
			game::camera->Pos.y -= 0.01f;
		}


		game::Draw(BlockShader, game::StaticRenderState);

		game::swapBuffers();
		game::windowSystemPollEvents();
		game::tick();
	}

	return 0;
}
