#pragma once

#include "Scene/GComponent.h"
#include <string>
#include <vector>

namespace GameEngine {

	class GEntity
	{
		std::uint32_t ID;
		std::string name;

		std::vector<Ref<Component>> components;


	};

}