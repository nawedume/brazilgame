#include <core.hpp>
#include <render.hpp>
#include <glad/glad.h>
#include <string>
#include <shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TOTAL_V_MEM 1000 * 1000 

namespace game {
	using namespace std;

	RenderState* StaticRenderState;
	RenderState* DynamicRenderState;

	struct {
		vector<string> TextureName;
		vector<GLuint> TextureRef;
	} Textures ;

	Camera* camera;

	void InitRenderState() {
		StaticRenderState = new RenderState { TOTAL_V_MEM, 0 };
		CreateVao(StaticRenderState, true); 

		DynamicRenderState = new RenderState { TOTAL_V_MEM, 0 };
		CreateVao(DynamicRenderState, false);

		camera = new Camera();
		camera->Pos = { 0.0, 0.0 };
		camera->Size = { CAMERA_WIDTH, CAMERA_HEIGHT };
	}

	void CreateVao(RenderState* state, bool isStatic) {
		glGenVertexArrays(1, &state->Vao);
		glBindVertexArray(state->Vao);
		glGenBuffers(1, &state->Vbo);
		glBindBuffer(GL_ARRAY_BUFFER, state->Vbo);

		GLuint type = isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
		glBufferData(GL_ARRAY_BUFFER, state->TotalSize * sizeof(Vertex), nullptr, type);
	}

	void RenderAddBlock(RenderState* state, Block block, vec3 baseColor) {
		Vertex quad[6] {};
		quad[0] = { .Pos = block.WorldPosStart, .Color = baseColor, .TexCoords = { 0.0, 0.0 } };
		quad[1] = { .Pos = { block.WorldPosEnd.x, block.WorldPosStart.y }, .Color = baseColor, .TexCoords = { 1.0, 0.0 } };
		quad[2] = { .Pos = block.WorldPosEnd, .Color = baseColor, .TexCoords = { 1.0, 1.0 } };

		quad[3] = { .Pos = block.WorldPosStart, .Color = baseColor, .TexCoords = { 0.0, 0.0 } };
		quad[4] = { .Pos = { block.WorldPosStart.x, block.WorldPosEnd.y }, .Color = baseColor, .TexCoords = { 0.0, 1.0 } };
		quad[5] = { .Pos = block.WorldPosEnd, .Color = baseColor, .TexCoords = { 1.0, 1.0 } };

		// @attention probably need to update so we batch
		glBindVertexArray(state->Vao);
		glBindVertexArray(state->Vbo);
		glBufferSubData(GL_ARRAY_BUFFER, state->CurrentSize * sizeof(Vertex), 6 * sizeof(Vertex), quad);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Color));
		glEnableVertexAttribArray(1);

		state->CurrentSize += 6;
	}

	void Draw(Shader* shader, RenderState* state) {
		shader->use();
		shader->setFloatMat3("uViewTransform", glm::value_ptr(camera->getViewMatrix())); 

		glBindVertexArray(state->Vao);
		glDrawArrays(GL_TRIANGLES, 0, state->CurrentSize);
	}
}
