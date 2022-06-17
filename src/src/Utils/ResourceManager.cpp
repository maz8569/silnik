#include "Utils/ResourceManager.h"

namespace GameEngine {

	std::map<std::string, Ref<GTexture>> ResourceManager::textures;
	std::map<std::string, Ref<Shader>> ResourceManager::shaders;
	std::map<std::string, Ref<Model>> ResourceManager::models;

	ResourceManager::ResourceManager()
	{
	}

	void ResourceManager::init()
	{
		loadShader("ourShader", "res/shaders/basic.vert", "res/shaders/basic.frag");
		loadShader("animatedShader", "res/shaders/basicAnim.vert", "res/shaders/basicAnim.frag");
		loadShader("refract", "res/shaders/refract.vert", "res/shaders/refract.frag");
		loadShader("water", "res/shaders/water.vert", "res/shaders/water.frag");
		loadShader("shadowMap", "res/shaders/shadowmapping.vert", "res/shaders/shadowmapping.frag");
		loadShader("foaMap", "res/shaders/shadowmappingBend.vert", "res/shaders/shadowmappingBend.frag");
		loadShader("debugDepth", "res/shaders/debugdepth.vert", "res/shaders/debugdepth.frag");

		loadModel("cube", std::string("res/models/cube/cube.obj"));
		loadModel("boat", std::string("res/models/statek/untitled.obj"));
		loadModel("house", std::string("res/models/dom/dom_p.obj"));
		loadModel("redhouse", std::string("res/models/reddom/reddom.obj"));
		loadModel("package", std::string("res/models/paczka/paczka.obj"));
		loadModel("redpackage", std::string("res/models/redpaczka/redpaczka.obj"));
		loadModel("bridge", std::string("res/models/lowpolymost/niby_most.obj"));
		loadModel("island", std::string("res/models/islandNew/wyspa.obj"));
		loadModel("ludek", std::string("res/models/ludek/ludek.fbx"));
		loadModel("testanimcol", std::string("res/models/testanim/gurl.fbx"));
		loadModel("testanim", std::string("res/models/testanim/testanim.dae"));
		loadModel("hidefCube", std::string("res/models/hidefCube/cube.obj"));
		loadModel("hidefPlane", std::string("res/models/hidefPlane/plane.obj"));
		loadModel("hidefPlane", std::string("res/models/hidefPlane/plane.obj"));
		loadModel("dirt", std::string("res/models/dirt/dirt.obj"));

		loadTexture("perlinoise", std::string("res/textures/perlinnoise.png"));
		loadTexture("back", std::string("res/textures/back.png"));
		loadTexture("hourglass", std::string("res/textures/hourglass.png"));
		loadTexture("box", std::string("res/textures/box.png"));
		loadTexture("numb2", std::string("res/textures/numb2.png"));
		loadTexture("numb1", std::string("res/textures/numb1.png"));
		loadTexture("numb0", std::string("res/textures/numb0.png"));
		loadTexture("fullheart", std::string("res/textures/fullheart.png"));
		loadTexture("exit", std::string("res/textures/buttonExit.png"));
		loadTexture("play", std::string("res/textures/buttonPlay.png"));
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

	Ref<Model> ResourceManager::loadModel(std::string name, string& path)
	{
		Ref<Model> model = CreateRef<Model>(path);

		models.insert(std::make_pair(name, model));
		
		return model;
	}


	Ref<Model> ResourceManager::getModel(std::string name)
	{
		return models.at(name);
	}

	Ref<GTexture> ResourceManager::loadTexture(std::string name, std::string& path)
	{
		Ref<GTexture> texture = CreateRef<GTexture>(path);

		textures.insert(std::make_pair(name, texture));
		
		return texture;
	}

	Ref<GTexture> ResourceManager::getTexture(std::string name)
	{
		return textures.at(name);
	}

	void ResourceManager::clear()
	{
		//shaders.clear();
	}

}