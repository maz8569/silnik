#pragma once

#include "GComponent.h"
#include <vector>
#include <glm/glm.hpp>

namespace GameEngine {

	class Boat : public GComponent
	{
	private:

		float m_speed;
		std::vector<glm::vec3> m_positions;
		float m_time = 0;


	public:

		int currentDestination;
		glm::vec3 pointDestination;
		glm::vec3 destVector;

		Boat(float speed, std::vector<glm::vec3>& positions);

		virtual void setParent(GObject* newParent);

		void changeDestination();

		virtual void Update(float dt);

	};

}