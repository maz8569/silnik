#pragma once

#include "MovingEntity.h"


namespace GameEngine {
	
	class Scene;

	class Vehicle : public MovingEntity
	{
	private:
		
		//a pointer to the world data enabling a vehicle to access any obstacle
		//path, wall, or agent data
		Scene* m_pScene;


	};

}
