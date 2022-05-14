#include "GUI/GuiComponent.h"

namespace GameEngine {

	bool GuiComponent::windowCh = true;

	unsigned int GuiComponent::m_scr_width = 1920, GuiComponent::m_src_height = 1080;

	GuiComponent::GuiComponent(std::string texture, glm::vec2 position, glm::vec2 scale, float rotation)
		: m_position(position), m_scale(scale), m_rotation(rotation)
	{
		m_texture = CreateRef<GTexture>(texture);
		m_transfrom_matrix = getTransformatiomMatrix();
	}

	GuiComponent::~GuiComponent()
	{
	}

	glm::mat4 GuiComponent::getTransformatiomMatrix()
	{
		if (GuiComponent::windowCh || isDirty) {

			float sx = m_scale.x / GuiComponent::m_scr_width;
			float sy = m_scale.y / GuiComponent::m_src_height;

			float x = m_position.x * 2 / GuiComponent::m_scr_width;
			float y = m_position.y * 2 / GuiComponent::m_src_height;
			y -= sy;

			//m_transfrom_matrix = glm::translate(glm::mat4(1.0f), { m_position.x / GuiComponent::m_scr_width, m_position.y / GuiComponent::m_src_height, 0 }) *
			m_transfrom_matrix = glm::translate(glm::mat4(1.0f), { x, y, 0 }) *
								 glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, -1)) *
								 glm::scale(glm::mat4(1.0f), { sx, sy, 1 });
		}

		return m_transfrom_matrix;
	}

	glm::mat4 GuiComponent::getTransformMatrix()
	{
		return m_transfrom_matrix;
	}

	void GuiComponent::setScrWidth(unsigned int scr_width)
	{
		GuiComponent::m_scr_width = scr_width;
		GuiComponent::windowCh = true;
	}

	void GuiComponent::setScrHeight(unsigned int scr_height)
	{
		GuiComponent::m_src_height = scr_height;
		GuiComponent::windowCh = true;
	}

	Ref<GTexture> GuiComponent::getTexture()
	{
		return m_texture;
	}

}