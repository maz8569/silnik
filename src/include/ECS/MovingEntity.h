#pragma once

#include <Scene/GComponent.h>
#include <glm/glm.hpp>

namespace GameEngine {

	class MovingEntity : public GComponent
	{
	protected:

		glm::vec2 m_vVelocity;

		//a normalized vector pointing in the direction the entity is heading.
		glm::vec2 m_vHeading;

		//a vector perpendicular to the heading vector
		glm::vec2 m_vSide;

		float m_dMass;

		//the maximum speed at which this entity may travel.
		float m_dMaxSpeed;

		//the maximum force this entity can produce to power itself
		//(think rockets and thrust)
		float m_dMaxForce;
		
		//the maximum rate (radians per second) at which this vehicle can rotate
		float m_dMaxTurnRate;

	public:

		MovingEntity(glm::vec2& velocity, float& maxSpeed, glm::vec2& heading, float mass, float turnRate, float maxForce);


		virtual void Update(float dt);

		float MaxSpeed();
		glm::vec2& Velocity();

	};

}
