#pragma once

#include <typedefs.h>
#include <glm/glm.hpp>

namespace GameEngine {

	class Sphere
	{
		glm::vec3 center;
		float radius;

		bool testSphereSphere(Ref<Sphere> other);
	};



}