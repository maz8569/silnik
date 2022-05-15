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

			float sx = m_scale.x * 2 / GuiComponent::m_scr_width;
			//float sx = m_scale.x;
			float sy = m_scale.y * 2 / GuiComponent::m_src_height;
			//float sy = m_scale.y;

			float x = m_position.x * 2 / GuiComponent::m_scr_width;
			//float x = m_position.x;
			float y = m_position.y * 2 / GuiComponent::m_src_height;
			//float y = GuiComponent::m_src_height - m_position.y;
			//y -= sy/2;
			

			//m_transfrom_matrix = glm::translate(glm::mat4(1.0f), { m_position.x / GuiComponent::m_scr_width, m_position.y / GuiComponent::m_src_height, 0 }) *
			m_transfrom_matrix = glm::translate(glm::mat4(1.0f), { x, y, 0 }) *
								 glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, -1)) *
								 glm::scale(glm::mat4(1.0f), { sx, sy, 1 });

			isDirty = false;
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

	glm::vec3& GuiComponent::getColor()
	{
		return color;
	}

	glm::vec3& GuiComponent::getHighLightColor()
	{
		return highlightColor;
	}

	float& GuiComponent::getOpacity()
	{
		return opacity;
	}

	glm::vec2 GuiComponent::getPosition()
	{
		return m_position;
	}

	glm::vec2 GuiComponent::getScale()
	{
		return m_scale;
	}

	void GuiComponent::onCollisionEnter(glm::vec2 pos)
	{
		color = highlightColor;
		isHovered = true;
	}

	void GuiComponent::onCollisionStay(glm::vec2 pos)
	{
	}

	void GuiComponent::onCollisionExit(glm::vec2 pos)
	{
		color = { 1, 1, 1 };
		isHovered = false;
	}

	void GuiComponent::onClick()
	{
		if (isHovered)
		{
			std::cout << "click";
		}
	}

	Ref<GTexture> GuiComponent::getTexture()
	{
		return m_texture;
	}

}