#include "Blocks/Layer.h"

using namespace GameEngine;

GameEngine::Layer::Layer(const glm::vec3 layer_position)
	: m_Position(layer_position), m_MeshState(LayerMeshState::Unbuilt), m_LayerState(LayerState::Ungenerated)
	, m_LayerFrustumAABB(glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), glm::vec3(layer_position.x * CHUNK_SIZE_X, layer_position.y* CHUNK_SIZE_Y, layer_position.z* CHUNK_SIZE_Z))
{
	memset(&m_LayerContents, BlockType::WATER, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
}

GameEngine::Layer::~Layer()
{

}

void GameEngine::Layer::SetBlock(BlockType type, const glm::vec3& position)
{
	Block b;
	b.m_blockType = type;

	m_LayerContents.at(position.x).at(position.y).at(position.z) = b;
}

void GameEngine::Layer::Construct()
{
	if (m_LayerMesh.ConstructMesh(this, m_Position))
	{
		m_MeshState = LayerMeshState::Built;
	}
	else
	{
		m_MeshState = LayerMeshState::Unbuilt;
	}
}

LayerMesh* GameEngine::Layer::GetLayerMesh()
{
	return &m_LayerMesh;
}

Block* GameEngine::Layer::GetBlock(int x, int y, int z)
{
	return &m_LayerContents[x][y][z];
}




