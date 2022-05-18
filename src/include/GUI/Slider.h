#pragma once

#include "GuiComponent.h"
#include <algorithm>

namespace GameEngine {

	class Slider : public GuiComponent
	{
	private:
		float* m_value;

		float m_minValue;
		float m_maxValue;
		
		Ref<MouseCursor> m_mouseCursor;
		Ref<Constraints> m_constraints;

		float m_valuePerPixel = 0;

		bool isDragged = false;

	public:
		Slider(float minValue, float maxValue, float* defaultValue, std::string texture, glm::vec2 position, glm::vec2 scale);
		
		virtual void onCollisionEnter(Ref<MouseCursor> cursor);
		virtual void onCollisionStay(Ref<MouseCursor> cursor);
		virtual void onCollisionExit(Ref<MouseCursor> cursor);

		virtual void Update();

		virtual void onClick();
		virtual void stopClick();
		
		void UpdatePosition(glm::vec2& pos);
		void UpdateValue();

		void setConstraints(Ref<Constraints> constraints);
		Ref<Constraints> getConstraints();

	};

}