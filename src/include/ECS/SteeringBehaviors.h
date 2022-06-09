#pragma once

#include <glm/glm.hpp>
#include <typedefs.h>

namespace GameEngine {

	class GObject;
	class Vehicle;

	class SteeringBehaviors
	{
	private:

		Ref<GObject> m_pTargetObj;

		Vehicle* m_pParent;

		glm::vec2 m_vTarget;

	public:

		SteeringBehaviors(Ref<GObject> targetObject, Vehicle* vehicle);

		glm::vec2 Calculate();

		glm::vec2 Seek(glm::vec2 target);

	};

}