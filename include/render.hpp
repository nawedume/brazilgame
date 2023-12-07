#pragma once
#include <core.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.hpp>

#define CAMERA_WIDTH 32
#define CAMERA_HEIGHT 32

namespace game {

	using namespace std;
	using namespace glm;

	struct RenderState {
		u32 TotalSize;
		u32 CurrentSize;
		u32 Vao;
		u32 Vbo;
	};

	struct Vertex {
		vec2 Pos;
		vec3 Color; // @todo Remove when adding the texture support
		vec2 TexCoords;
	};

	struct Camera {
		vec2 Pos;
		vec2 Size; 

		mat3 getViewMatrix() {
			mat3 view (1.0f);
			view[2] = { -1.0, -1.0, 1.0 };
			view[0][0] = 2.0f / Size.x;
			view[1][1] = 2.0f / Size.y;

			mat3 cameraTranslate (1.0f);
			cameraTranslate[2] = { -Pos.x, -Pos.y, 1.0f };

			return view * cameraTranslate;
		}
	};

	extern Camera* camera;

	extern RenderState* StaticRenderState;
	extern RenderState* DynamicRenderState;

	extern void CreateVao(RenderState* state, bool isStatic);

	extern void InitRenderState();

	extern void RenderAddBlock(RenderState* state, Block block, vec3 baseColor);

	extern void Draw(Shader* shader, RenderState* state);
};

