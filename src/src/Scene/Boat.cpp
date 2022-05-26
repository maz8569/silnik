#include "Scene/Boat.h"
#include <Scene/GObject.h>
#include <iostream>

namespace GameEngine {

	Boat::Boat(float speed, std::vector<glm::vec3>& positions)
		: GComponent(), m_speed(speed), m_positions(positions)
	{
	}

	void Boat::setParent(GObject* newParent)
	{
		GComponent::setParent(newParent);
		if (m_positions.size() > 0)
		{
			parent->set_local_position(m_positions[0]);
			currentDestination = 0;

		}
		else
		{
			currentDestination = -1;
		}

		changeDestination();
	}

	void Boat::changeDestination()
	{
		if (currentDestination != -1)
		{
			glm::vec3 oldDestination = m_positions[currentDestination];
			if (currentDestination + 1  == m_positions.size())
			{
				currentDestination = 0;
			}
			else
			{
				currentDestination++;
			}
			pointDestination = m_positions[currentDestination];

			destVector = glm::normalize(pointDestination - oldDestination);
			//destVector = pointDestination - oldDestination;
			//std::cout << "\n vector: " << destVector.x << " " << destVector.y << " " << destVector.z << " \n";

			glm::vec3 currentRotation = parent->get_transform().m_rotation;
			float yRot = atan2(destVector.x, destVector.z);
			yRot = 360-glm::degrees(yRot);

			parent->set_local_rotationY(yRot);
			//std::cout << yRot;
		}
	}

	void Boat::Update(float dt)
	{
		m_time += dt;

		if (m_time > 360)
		{
			m_time = 0;
		}

		float rx = (sinf(m_time * m_speed) * 3);

		parent->set_local_rotationX(rx);
		parent->set_local_rotationZ(-rx);

		if (glm::length(parent->get_transform().m_position - pointDestination) < 0.1f)
		{
			changeDestination();
		}

		glm::vec3 newPos = parent->get_transform().m_position;
		newPos += destVector * glm::vec3(m_speed * dt);
		parent->set_local_position(newPos);

	}
}