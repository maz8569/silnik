#include "Scene/Particle.h"

namespace GameEngine {

	Particle::Particle()
		: pos(0.0f), speed(0.0f), color(0.0f), size(0.0f), life(0.0f)
	{

	}

	Particle::Particle(glm::vec3 position, glm::vec3 velocity, glm::vec4 Color, float s, float l)
		: pos(position), speed(velocity), color(Color), size(s), life(l)
	{
	}

	bool Particle::Update(float dt)
	{

		glm::vec3 change = speed;
		change *= dt;

		pos += change;

		elapsedTime += dt;

		return elapsedTime < life;
	}

}