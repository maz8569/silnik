#include "Blocks/Chunk.h"

using namespace GameEngine;

GameEngine::Chunk::Chunk(const glm::vec3 chunk_position)
	: m_Position(chunk_position), m_MeshState(ChunkMeshState::Unbuilt), m_ChunkState(ChunkState::Ungenerated)
	, m_ChunkFrustumAABB(glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), glm::vec3(chunk_position.x * CHUNK_SIZE_X, chunk_position.y* CHUNK_SIZE_Y, chunk_position.z* CHUNK_SIZE_Z))
{
	memset(&m_ChunkContents, BlockType::WATER, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
}

GameEngine::Chunk::~Chunk()
{

}

void GameEngine::Chunk::SetBlock(BlockType type, const glm::vec3& position)
{
	Block b;
	b.m_blockType = type;

	m_ChunkContents.at(position.x).at(position.y).at(position.z) = b;
}

void GameEngine::Chunk::Construct()
{
	if (m_ChunkMesh.ConstructMesh(this, m_Position))
	{
		m_MeshState = ChunkMeshState::Built;
	}
	else
	{
		m_MeshState = ChunkMeshState::Unbuilt;
	}
}

ChunkMesh* GameEngine::Chunk::GetChunkMesh()
{
	return &m_ChunkMesh;
}

Block* GameEngine::Chunk::GetBlock(int x, int y, int z)
{
	return &m_ChunkContents[x][y][z];
}




