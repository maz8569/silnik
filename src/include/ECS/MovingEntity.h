#pragma once

#include <Scene/GObject.h>
#include <glm/glm.hpp>

namespace GameEngine {

	class MovingEntity : public GObject
	{
	protected:

		glm::vec2 m_vVelocity;

		//a normalized vector pointing in the direction the entity is heading.
		glm::vec2 m_vHeading;

		//a vector perpendicular to the heading vector
		glm::vec2 m_vSide;

		double m_dMass;

		//the maximum speed at which this entity may travel.
		double m_dMaxSpeed;

		//the maximum force this entity can produce to power itself
		//(think rockets and thrust)
		double m_dMaxForce;
		
		//the maximum rate (radians per second) at which this vehicle can rotate
		double m_dMaxTurnRate;

	public:


	};

}
