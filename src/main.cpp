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

using namespace glm;
using namespace std;

bool isValidMove(ivec2 position, game::Grid* grid) {
	game::CellRef* ref = grid->get(position.y, position.x); 
	return game::isInBounds(position, grid) && holds_alternative<game::Grass>(*ref);
}

int main() {
	game::setupWindow();
	game::setupGlad();

	game::InitRenderState();

	f32 PreviousTime = 0.0f;

	game::Grid myGrid(4, 4);
	game::Player player = { .Dir = X_AXIS };
	myGrid.set(0, 0, player);

	game::Scenery scenery = { };
	myGrid.set(0, 3, scenery);
	myGrid.set(2, 3, scenery);
	myGrid.set(3, 3, scenery);

	game::ControllerFn fn = [](ivec2 pos, game::Grid* grid, u32 step) {
		ivec2 const rotations[4] = { -X_AXIS, -Y_AXIS, -X_AXIS, -Y_AXIS};
		ivec2 newPos = game::CommonAttack(pos, grid, step);

		if (pos == newPos) {
			game::CommonRotate(rotations, newPos, grid, step);
		}
	};
	game::Chu chu = { .Dir = -X_AXIS, .Func = fn };
	myGrid.set(3, 2, chu);

	ivec2 currentPlayerPos = { 0, 0 };
	u32 step = 0;
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

		ivec2 newPlayerPos = currentPlayerPos;
		if (game::isKeyClicked(game::KEY_D)) {
			newPlayerPos += X_AXIS;
		}
		else if (game::isKeyClicked(game::KEY_A)) {
			newPlayerPos -= X_AXIS;
		}
		else if (game::isKeyClicked(game::KEY_W)) {
			newPlayerPos += Y_AXIS;
		}
		else if (game::isKeyClicked(game::KEY_S)) {
			newPlayerPos -= Y_AXIS;
		}

		if (newPlayerPos != currentPlayerPos && isValidMove(newPlayerPos, &myGrid)) {
			myGrid.remove(currentPlayerPos.y, currentPlayerPos.x);
			myGrid.set(newPlayerPos.y, newPlayerPos.x, player);
			currentPlayerPos = newPlayerPos;
			step += 1;

			for (u32 row = 0; row < myGrid.Height; row++) {
				for (u32 col = 0; col < myGrid.Width; col++) {
					game::CellRef* ref = myGrid.get(row, col);
					if (holds_alternative<game::Chu>(*ref)) {
						game::Chu chu = get<game::Chu>(*ref);
						chu.Func({ col, row }, &myGrid, step);
					}
				}
			}
		}


		game::DrawGrid(&myGrid);
		game::swapBuffers();
		game::windowSystemPollEvents();
		game::tick();
	}

	return 0;
}
