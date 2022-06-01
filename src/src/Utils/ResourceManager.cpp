#include "Utils/ResourceManager.h"

namespace GameEngine {

	std::map<std::string, Ref<GTexture>> ResourceManager::textures;
	std::map<std::string, Ref<Shader>> ResourceManager::shaders;

	ResourceManager::ResourceManager()
	{
		//shaders.insert(std::make_pair("particleShader", CreateRef<Shader>("res/shaders/particle.vert", "res/shaders/particle.frag")));
	}

	Ref<Shader> ResourceManager::loadShader(std::string name, const char* vertexPath, const char* fragmentPath, const char* geometryPath)
	{
		Ref<Shader> shader = CreateRef<Shader>(vertexPath, fragmentPath, geometryPath);
		
		shaders.insert(std::make_pair(name, shader));

		return shader;
	}

	Ref<Shader> ResourceManager::getShader(std::string name)
	{
		return shaders.at(name);
	}

	void ResourceManager::clear()
	{

	}

}