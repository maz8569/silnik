#include "GUI/GuiComponent.h"

namespace GameEngine {

	GameEngine::GuiComponent::GuiComponent(std::string texture, glm::vec2 position, glm::vec2 scale)
		: m_position(position), m_scale(scale)
	{
		m_texture = CreateRef<BasicTexture>(texture);
	}

	GameEngine::GuiComponent::~GuiComponent()
	{
	}

	Ref<BasicTexture> GameEngine::GuiComponent::getTexture()
	{
		return m_texture;
	}

}