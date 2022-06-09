#include "ECS/SteeringBehaviors.h"
#include <ECS/Vehicle.h>

namespace GameEngine {

	SteeringBehaviors::SteeringBehaviors(Ref<GObject> targetObject, Vehicle* vehicle)
		: m_pTargetObj(targetObject), m_pParent(vehicle)
	{
		m_vTarget = glm::vec2();
	}


	glm::vec2 SteeringBehaviors::Calculate()
	{
		m_vTarget.x = m_pTargetObj->get_transform().m_position.x;
		m_vTarget.y = m_pTargetObj->get_transform().m_position.z;

		
		return Seek(m_vTarget);
	}

	glm::vec2 SteeringBehaviors::Seek(glm::vec2 target)
	{
		glm::vec2 desiredVelocity = glm::normalize(target - m_pParent->vehiclePos) * m_pParent->MaxSpeed();
		
		return (desiredVelocity - m_pParent->Velocity());
	}

}