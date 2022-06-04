#pragma once

#include "MovingEntity.h"
#include <Physics/Collisions/Collision.h>
#include "SteeringBehaviors.h"

namespace GameEngine {

	class Vehicle : public MovingEntity
	{
	private:
		
		//a pointer to the world data enabling a vehicle to access any obstacle
		//path, wall, or agent data
		//Ref<Collision> m_collisions;
		Ref<SteeringBehaviors> m_pSteering;

	public:

		glm::vec2 vehiclePos;

		Vehicle(glm::vec2& velocity, float maxSpeed, glm::vec2& heading, float mass, float turnRate, float maxForce);

		virtual void Update(float dt);

		void setVehiclePos();

		void addSteeringBehavior(Ref<GObject> targetObject);

	};

}
