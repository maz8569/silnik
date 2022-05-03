#include <Physics/Collisions/Sphere.h>

namespace GameEngine {
	bool Sphere::testSphereSphere(Ref<Sphere> other)
	{
		glm::vec3 d = center - other->center;
		float dist2 = glm::dot(d, d);
		// Spheres intersect if squared distance is less than squared sum of radii
		float radiusSum = radius + other->radius;
		return dist2 <= radiusSum * radiusSum;
	}

}

