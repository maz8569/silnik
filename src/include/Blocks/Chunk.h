#pragma once

#include <typedefs.h>
#include <glm/glm.hpp>
#include <array>
#include "ChunkMesh.h"
#include <Rendering/Frustum.h>

namespace GameEngine {

	enum class ChunkMeshState : std::uint8_t
	{
		Built = 0,
		Unbuilt,
		error
	};

	enum class ChunkState
	{
		Ungenerated = 5,
		Generated,
		Changed,
		undefined
	};

	class Chunk {
	public:

		Chunk(const glm::vec3 chunk_position);
		~Chunk();

		void SetBlock(BlockType type, const glm::vec3& position);

		void Construct();
		ChunkMesh* GetChunkMesh();

		Block* GetBlock(int x, int y, int z);

		const glm::vec3 m_Position;
		ChunkMeshState m_MeshState;
		ChunkState m_ChunkState = ChunkState::Ungenerated;
		std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z> m_ChunkContents;
		FrustumAABB m_ChunkFrustumAABB;

	private:
		ChunkMesh m_ChunkMesh;
	};

}
