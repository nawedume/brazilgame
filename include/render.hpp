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
		GLuint Vao;
		GLuint Vbo;
	};

	struct Vertex {
		vec2 Pos;
		vec3 Color; // @todo Remove when adding the texture support
		vec2 TexCoord;
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

	struct DrawInfo {
		u32 BufferIndex;
		GLuint TextureRef;
	};

	extern DrawInfo PlayerDrawInfo;
	extern DrawInfo ChuDrawInfo;
	extern DrawInfo TreeDrawInfo;
	extern DrawInfo GrassDrawInfo;
	extern DrawInfo NextLevelPortalDrawInfo;
	extern DrawInfo GoatDrawInfo;
	extern DrawInfo WaterDrawInfo;
	extern DrawInfo RockDrawInfo;

	extern void CreateVao(RenderState* state, bool isStatic);

	extern void InitRenderState();

	extern u32 RenderAddBlock(RenderState* state, Block block, vec3 baseColor);

	extern u32 RenderAddBlock(RenderState* state, Block block, vec3 baseColor, vec2 texCoord);

	extern void Draw(DrawInfo& drawInfo, vec2 position, vec2 scale);

	extern void Draw(DrawInfo& drawInfo, vec2 position, vec2 scale, vec2 textureOffset);

	extern void DrawBackground(DrawInfo& bgInfo);
};

