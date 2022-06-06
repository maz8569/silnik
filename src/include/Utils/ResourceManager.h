#pragma once

#include <map>
#include <Texture/Texture.h>
#include <typedefs.h>
#include <Shaders/Shader.h>
#include <Rendering/Model.h>

namespace GameEngine {

	class ResourceManager
	{
	public:
		static std::map<std::string, Ref<GTexture>> textures;
		static std::map<std::string, Ref<Shader>> shaders;
		static std::map<std::string, Ref<Model>> models;

		ResourceManager();

		static void init();
		static Ref<Shader> loadShader(std::string name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
		static Ref<Shader> getShader(std::string name);

		static Ref<Model> loadModel(std::string name, string& path);
		static Ref<Model> getModel(std::string name);

		static Ref<GTexture> loadTexture(std::string name, std::string& path);
		static Ref<GTexture> getTexture(std::string name);

		static void clear();
	};

}