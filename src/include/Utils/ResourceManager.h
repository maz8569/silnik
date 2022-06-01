#pragma once

#include <map>
#include <Texture/Texture.h>
#include <typedefs.h>
#include <Shaders/Shader.h>

namespace GameEngine {

	class ResourceManager
	{
	public:
		static std::map<std::string, Ref<GTexture>> textures;
		static std::map<std::string, Ref<Shader>> shaders;

		ResourceManager();

		static Ref<Shader> loadShader(std::string name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
		static Ref<Shader> getShader(std::string name);

		static void clear();
	};

}