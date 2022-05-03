#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <typedefs.h>

#include "Block.h"
#include <Utils/VertexArray.h>
#include <Utils/VertexBuffer.h>
#include <Utils/IndexBuffer.h>

namespace GameEngine {

	class Chunk;
	typedef std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z>* ChunkDataTypePtr;

	ChunkDataTypePtr m_GetChunkDataForMeshing(int cx, int cz);

	class ChunkMesh
	{
	public:

		ChunkMesh();
		~ChunkMesh();

		bool ConstructMesh(Chunk* layer, const glm::vec3& chunk_pos);

		std::uint32_t m_VerticesCount;
		std::uint32_t m_TransparentVerticesCount;
		std::uint32_t m_ModelVerticesCount;
		VertexArray m_VAO;
		VertexArray m_TransparentVAO;
		VertexArray m_ModelVAO;
	private:

		void AddFace(Chunk* layer, BlockFaceType face_type, const glm::vec3& position, BlockType type, uint8_t light_level = 0,
			bool buffer = true);

		std::vector<Vertex> m_Vertices;
		std::vector<Vertex> m_TransparentVertices;
		std::vector<Vertex> m_ModelVertices;
		glm::vec4 m_TopFace[4];
		glm::vec4 m_BottomFace[4];
		glm::vec4 m_ForwardFace[4];
		glm::vec4 m_BackFace[4];
		glm::vec4 m_LeftFace[4];
		glm::vec4 m_RightFace[4];

		VertexBuffer m_VBO;
		VertexBuffer m_TransparentVBO; // Vertex buffer for trasparent blocks
		VertexBuffer m_ModelVBO; // Vertex buffer for trasparent blocks
	};

}