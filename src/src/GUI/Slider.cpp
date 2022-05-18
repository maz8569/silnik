#include "GUI/Slider.h"

namespace GameEngine {



	Slider::Slider(float minValue, float maxValue, float* defaultValue, std::string texture, glm::vec2 position, glm::vec2 scale)
		: GuiComponent(texture, position, scale), m_minValue(minValue), m_maxValue(maxValue), m_value(defaultValue)
	{
		setHighlightColor({ 0, 0, 1 });
		m_constraints = nullptr;
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
			glm::vec2 newPos = m_mouseCursor->mousePos;
			UpdatePosition(newPos);
			UpdateValue();
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

	void Slider::UpdatePosition(glm::vec2& pos)
	{
		if (m_constraints != nullptr)
		{
			if (m_constraints->Xlock == true)
			{
				pos.x = getPosition().x;
			}
			else
			{
				pos.x = std::clamp(pos.x, m_constraints->Xrange.x, m_constraints->Xrange.y);
			}
			if (m_constraints->Ylock == true)
			{
				pos.y = getPosition().y;
			}
			else
			{
				pos.y = std::clamp(pos.y, m_constraints->Yrange.x, m_constraints->Yrange.y);
			}
		}
		setPosition(pos);
	}

	void Slider::UpdateValue()
	{
		float value = getPosition().x;

		if (m_constraints->Ylock)
		{
			value = getPosition().x - m_constraints->Xrange.x;
			value = value / m_constraints->Xdist;
			value *= (m_maxValue - m_minValue);
			value += m_minValue;
		}

		//value = std::clamp(value, m_minValue, m_maxValue);

		*m_value = value;
	}

	void Slider::setConstraints(Ref<Constraints> constraints)
	{
		m_constraints = constraints;
		UpdateValue();
	}

	Ref<Constraints> Slider::getConstraints()
	{
		return m_constraints;
	}

}