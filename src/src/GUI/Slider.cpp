#include "GUI/Slider.h"

namespace GameEngine {



	Slider::Slider(float minValue, float maxValue, float* defaultValue, std::string texture, glm::vec2 position, glm::vec2 scale)
		: GuiComponent(texture, position, scale), m_minValue(minValue), m_maxValue(maxValue), m_value(defaultValue)
	{
		setHighlightColor({ 0, 0, 1 });
	}

	void Slider::onCollisionEnter(Ref<MouseCursor> cursor)
	{
		GuiComponent::onCollisionEnter(cursor);
		m_mouseCursor = cursor;
	}

	void Slider::onCollisionStay(Ref<MouseCursor> cursor)
	{
	}

	void Slider::onCollisionExit(Ref<MouseCursor> cursor)
	{
		GuiComponent::onCollisionExit(cursor);
	}

	void Slider::Update()
	{
		if (isDragged && m_mouseCursor != nullptr)
		{
			setPosition(m_mouseCursor->mousePos);
			*m_value = m_mouseCursor->mousePos.x;
		}
	}

	void Slider::onClick()
	{
		if (getIsHovered())
		{
			std::cout << "click";
			isDragged = true;
		}
	}

	void Slider::stopClick()
	{
		isDragged = false;
		m_mouseCursor = nullptr;
	}

}