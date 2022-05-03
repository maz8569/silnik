#pragma once

#include <Utils/VertexArray.h>
#include <Utils/VertexBuffer.h>
#include <Shaders/Shader.h>
#include <Texture/Texture.h>
#include <Scene/FPcam.h>
#include <Blocks/Chunk.h>

namespace GameEngine {

	class BlockRenderer
	{
	public:

		BlockRenderer();

		void StartChunkRendering(FPcam* camera, const glm::vec4& ambient_light, int render_distance, const glm::vec4& sun_position);
		void RenderTransparentChunk(Chunk* chunk);
		void RenderChunk(Chunk* chunk);
		void EndChunkRendering();

		void StartChunkModelRendering(FPcam* camera, const glm::vec4& ambient_light, int render_distance, const glm::vec4& sun_position);
		void RenderChunkModels(Chunk* chunk);
		void EndChunkModelRendering();

		Texture* GetAtlasTexture() { return &m_BlockAtlas; }

	private:
		VertexBuffer m_VBO;
		VertexArray m_VAO;
		Shader m_DefaultChunkShader;
		Texture m_BlockAtlas;
	};

}