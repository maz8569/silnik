#pragma once

#include <glm/glm.hpp>

namespace GameEngine {

	class Constraints
	{
	public:

		bool Xlock = false;
		bool Ylock = false;

		glm::vec2 Xrange;
		glm::vec2 Yrange;

		float Xdist;
		float Ydist;

		Constraints(glm::vec2 newXdist, glm::vec2 newYdist, bool newXlock = false, bool newYlock = false);

	};

}