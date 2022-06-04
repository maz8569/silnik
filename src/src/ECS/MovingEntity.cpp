#include "ECS/MovingEntity.h"

GameEngine::MovingEntity::MovingEntity(glm::vec2& velocity, float& maxSpeed, glm::vec2& heading, float mass, float turnRate, float maxForce)
	: GComponent(), m_vVelocity(velocity), m_vHeading(heading), m_vSide(-heading.y, heading.x), m_dMaxSpeed(maxSpeed), m_dMass(mass), m_dMaxTurnRate(turnRate), m_dMaxForce(maxForce)
{
}

void GameEngine::MovingEntity::Update(float dt)
{
}

float GameEngine::MovingEntity::MaxSpeed()
{
	return m_dMaxSpeed;
}

glm::vec2& GameEngine::MovingEntity::Velocity()
{
	return m_vVelocity;
}
