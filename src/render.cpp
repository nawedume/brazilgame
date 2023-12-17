#include <core.hpp>
#include <render.hpp>
#include <glad/glad.h>
#include <stdexcept>
#include <string>
#include <shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TOTAL_V_MEM 1000 * 1000 

namespace game {
	using namespace std;

	RenderState* WorldRenderState;
	Shader* BlockShader;
	Shader* BackgroundShader;

	DrawInfo PlayerDrawInfo;
	DrawInfo ChuDrawInfo;
	DrawInfo TreeDrawInfo;
	DrawInfo GrassDrawInfo;
	DrawInfo NextLevelPortalDrawInfo;
	DrawInfo GoatDrawInfo;
	DrawInfo WaterDrawInfo;
	DrawInfo RockDrawInfo;
	Camera* camera;

	GLuint GenerateTexture(string const& filePath, bool useLinearFilter = false) {
		s32 width, height, nComponents;
		stbi_set_flip_vertically_on_load(true);
		u8* data = stbi_load(filePath.c_str(), &width, &height, &nComponents, 0);
		if (data) {
			assert(nComponents == 4);

			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			if (useLinearFilter) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			} else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			stbi_image_free(data);
			return texture;
		} else {
			fprintf(stderr, "Could not load file\n");
			throw std::runtime_error("Error can't load file\n");
		}
	}

	void InitRenderState() {
		WorldRenderState = new RenderState { TOTAL_V_MEM, 0 };
		CreateVao(WorldRenderState, true); 

		camera = new Camera();
		camera->Pos = { 0.0, 0.0 };
		camera->Size = { CAMERA_WIDTH, CAMERA_HEIGHT };

		BlockShader = new Shader("./shaders/block.vs", "./shaders/block.fs");
		BackgroundShader = new Shader("./shaders/bg.vs", "./shaders/bg.fs");

		TreeDrawInfo.BufferIndex = RenderAddBlock(WorldRenderState, { .WorldPosStart = vec2(0.0), .WorldPosEnd = vec2(1.0, 1.0) }, { 0.1, 1.0, 1.0 });
		TreeDrawInfo.TextureRef = GenerateTexture("./assets/Tree.png");

		PlayerDrawInfo.BufferIndex = RenderAddBlock(WorldRenderState, { .WorldPosStart = vec2(0.0), .WorldPosEnd = vec2(1.0, 1.0) }, { 0.1, 0.1, 0.6 });
		PlayerDrawInfo.TextureRef = GenerateTexture("./assets/Man.png");

		ChuDrawInfo.BufferIndex = RenderAddBlock(WorldRenderState, { .WorldPosStart = vec2(0.0), .WorldPosEnd = vec2(1.0, 1.0) }, { 1.0, 0.0, 0.3 }, { 1.0, 0.25 });
		ChuDrawInfo.TextureRef = GenerateTexture("./assets/Chu.png");

		GrassDrawInfo.BufferIndex = RenderAddBlock(WorldRenderState, { .WorldPosStart = vec2(-1.0, -1.0), .WorldPosEnd = vec2(1.0, 1.0) }, { 0.4, 0.0, 0.6 }, { 1.0, 1.0 });
		GrassDrawInfo.TextureRef = GenerateTexture("./assets/Grass.png", true);

		NextLevelPortalDrawInfo.BufferIndex = RenderAddBlock(WorldRenderState, { .WorldPosStart = vec2(0.0), .WorldPosEnd = vec2(1.0, 1.0) }, { 1.0, 1.0, 1.0 });
		NextLevelPortalDrawInfo.TextureRef = GenerateTexture("./assets/Portal.png");

		GoatDrawInfo.BufferIndex = RenderAddBlock(WorldRenderState, { .WorldPosStart = vec2(0.0), .WorldPosEnd = vec2(1.0, 1.0) }, { 1.0, 1.0, 1.0 },  { 0.25, 1.0 });
		GoatDrawInfo.TextureRef = GenerateTexture("./assets/Goat.png");

		WaterDrawInfo.BufferIndex = RenderAddBlock(WorldRenderState, { .WorldPosStart = vec2(0.0), .WorldPosEnd = vec2(1.0, 1.0) }, { 0.0, 0.0, 1.0 });
		WaterDrawInfo.TextureRef = GenerateTexture("./assets/Water.png");

		RockDrawInfo.BufferIndex = RenderAddBlock(WorldRenderState, { .WorldPosStart = vec2(0.0), .WorldPosEnd = vec2(1.0, 1.0) }, { 0.0, 0.0, 1.0 });
		RockDrawInfo.TextureRef = GenerateTexture("./assets/Rock.png");
	}

	void CreateVao(RenderState* state, bool isStatic) {
		glGenVertexArrays(1, &state->Vao);
		glBindVertexArray(state->Vao);
		glGenBuffers(1, &state->Vbo);
		glBindBuffer(GL_ARRAY_BUFFER, state->Vbo);

		GLuint type = isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
		glBufferData(GL_ARRAY_BUFFER, state->TotalSize * sizeof(Vertex), nullptr, type);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Color));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, TexCoord));
		glEnableVertexAttribArray(2);
	}

	u32 RenderAddBlock(RenderState* state, Block block, vec3 baseColor, vec2 texCoord) {
		Vertex quad[6] {};
		quad[0] = { .Pos = block.WorldPosStart, .Color = baseColor, .TexCoord = { 0.0, 0.0 } };
		quad[1] = { .Pos = { block.WorldPosEnd.x, block.WorldPosStart.y }, .Color = baseColor, .TexCoord = { texCoord.x, 0.0 } };
		quad[2] = { .Pos = block.WorldPosEnd, .Color = baseColor, .TexCoord = texCoord };

		quad[3] = { .Pos = block.WorldPosStart, .Color = baseColor, .TexCoord = { 0.0, 0.0 } };
		quad[4] = { .Pos = block.WorldPosEnd, .Color = baseColor, .TexCoord = texCoord };
		quad[5] = { .Pos = { block.WorldPosStart.x, block.WorldPosEnd.y }, .Color = baseColor, .TexCoord = { 0.0, texCoord.y } };

		// @attention probably need to update so we batch
		glBindVertexArray(state->Vao);
		glBindBuffer(GL_ARRAY_BUFFER, state->Vbo);
		glBufferSubData(GL_ARRAY_BUFFER, state->CurrentSize * sizeof(Vertex), 6 * sizeof(Vertex), quad);

		u32 idx = state->CurrentSize;
		state->CurrentSize += 6;
		return idx;
	}

	u32 RenderAddBlock(RenderState* state, Block block, vec3 baseColor) {
		return RenderAddBlock(state, block, baseColor, vec2(1.0, 1.0));
	}

	void Draw(DrawInfo& drawInfo, vec2 position, vec2 scale) {
		Draw(drawInfo, position, scale, vec2(0.0f));
	}

	void Draw(DrawInfo& drawInfo, vec2 position, vec2 scale, vec2 textureOffset) {
		BlockShader->use();

		BlockShader->setVec2("uPos", value_ptr(position));
		BlockShader->setVec2("uScale", value_ptr(scale));
		BlockShader->setVec2("uTexOffset", value_ptr(textureOffset));

		glBindTexture(GL_TEXTURE_2D, drawInfo.TextureRef);

		glBindVertexArray(WorldRenderState->Vao);
		glDrawArrays(GL_TRIANGLES, drawInfo.BufferIndex, 6);
	}

	void DrawBackground(DrawInfo& bgInfo) {
		BackgroundShader->use();
		glBindTexture(GL_TEXTURE_2D, bgInfo.TextureRef);
		glBindVertexArray(WorldRenderState->Vao);
		glDrawArrays(GL_TRIANGLES, bgInfo.BufferIndex, 6);

	}
}
