#include "Scene/Scene.h"

using namespace GameEngine;

GameEngine::Scene::Scene(std::string name): m_name(name)
{
	m_camera = CreateRef<Camera>(Camera(glm::vec3(0.0f, 10.0f, 0.0f)));
	m_camera->Pitch = -90;
	m_camera->Yaw = 90;

	m_root = CreateRef<GObject>();
}

void GameEngine::Scene::Start()
{
}

void GameEngine::Scene::Update()
{
	m_root->update(m_root->get_transform(), true);
	for (auto& obj : m_GObjects)
	{
		obj->Update();
	}
	m_camera->Move();

}

void GameEngine::Scene::FixedUpdate()
{
}

void GameEngine::Scene::LateUpdate()
{
}

void GameEngine::Scene::Render(Ref<Shader> shader)
{
	checkFrustum();


	for (auto& obj : m_seenGObjects)
	{
		obj->render(shader);
	}

	m_skybox.RenderSkybox(m_camera);
}

void GameEngine::Scene::checkFrustum()
{
	m_camera->updateFrustum();
	m_seenGObjects.clear();
	for (auto& obj : m_GObjects)
	{
		if (obj->getAABB() != nullptr)
		{
			if (m_camera->m_frustum.BoxInFrustum(obj->getAABB()))
			{
				m_seenGObjects.push_back(obj);
			}
		}
	}
}

void GameEngine::Scene::addObjectToScene(Ref<GObject> obj)
{
	if (obj != nullptr)
	{
		m_root->add_child(obj);
		obj->add_parent(m_root);

		m_GObjects.push_back(obj);
	}
}

Ref<GObject> GameEngine::Scene::getObjectByName(const std::string& name)
{
	return Ref<GObject>();
}

std::vector<Ref<GObject>> GameEngine::Scene::getAllObjects()
{
	return std::vector<Ref<GObject>>();
}

std::vector<Ref<GObject>> GameEngine::Scene::getAllObjectsByName(const std::string& name)
{
	return std::vector<Ref<GObject>>();
}

Ref<GObject> GameEngine::Scene::getObjectByID(unsigned int ID)
{
	return Ref<GObject>();
}
