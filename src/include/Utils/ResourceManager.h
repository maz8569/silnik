#pragma once

#include <map>
#include <Texture/Texture.h>
#include <typedefs.h>

namespace GameEngine {

	class ResourceManager
	{
	public:
		std::map<std::string, Ref<GTexture>> textures;
	};

}