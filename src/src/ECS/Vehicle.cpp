#include "ECS/Vehicle.h"
#include <Scene/GObject.h>


namespace GameEngine {

	Vehicle::Vehicle(glm::vec2& velocity, float maxSpeed, glm::vec2& heading, float mass, float turnRate, float maxForce)
		: MovingEntity(velocity, maxSpeed, heading, mass, turnRate, maxForce)
	{
	}

	void GameEngine::Vehicle::Update(float dt)
	{
		setVehiclePos();

		glm::vec2 steeringForce = m_pSteering->Calculate();
		glm::vec2 acceleration = steeringForce / (float)m_dMass;
		m_vVelocity += acceleration * dt;
		parent->increment_local_position(glm::vec3(m_vVelocity.x, 0, m_vVelocity.y) * dt);

		if (m_vVelocity.length() > 0.0001)
		{
			m_vHeading = glm::normalize(m_vVelocity);
			m_vSide.x = -m_vHeading.y;
			m_vSide.y = m_vHeading.x;
		}

		setVehiclePos();

	}

	void Vehicle::setVehiclePos()
	{
		vehiclePos.x = parent->get_transform().m_position.x;
		vehiclePos.y = parent->get_transform().m_position.z;
	}

	void Vehicle::addSteeringBehavior(Ref<GObject> targetObject)
	{
		m_pSteering = CreateRef<SteeringBehaviors>(targetObject, this);
	}

}