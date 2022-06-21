#include "Scene/Scene.h"
#include <Physics/Collisions/Collision.h>
#include <Rendering/WindowManager.h>

namespace GameEngine {

	Scene::Scene(std::string name) : m_name(name)
	{
		m_camera = CreateRef<Camera>(glm::vec3(0.0f, 10.0f, 0.0f));
		m_camera->Pitch = -55;
		m_camera->Yaw = -90;

		m_skybox = CreateRef<Skybox>();
		colMan = CreateRef<Collision>();
		textRenderer = CreateRef<TextRenderer>(m_camera, "res/fonts/PressStart2P.ttf", "res/shaders/GUIText.vert", "res/shaders/GUIText.frag");
		if (!textRenderer->success)
		{
			exit(-1);
		}

		m_root = CreateRef<GObject>();
	}

	void Scene::Start()
	{
	}

	void Scene::Update(float dt)
	{

		gameManager->Update(dt);

		m_root->update_transform(m_root->get_transform(), true);
		for (i = 0; i < m_GObjects.size(); i++)
		{
			auto& obj = m_GObjects[i];
			obj->Update(dt);
		}
		m_camera->Move();

		guiManager->Update();
		colMan->CollisionCheck();
	}

	void Scene::FixedUpdate()
	{
	}

	void Scene::LateUpdate()
	{
	}

	void Scene::Render(Ref<Shader> shader)
	{
		checkFrustum();


		for (auto& obj : m_seenGObjects)
		{
			Ref<Shader> s = obj->shader;
			if (s != nullptr)

				obj->render(s);
		}

		m_skybox->RenderSkybox(m_camera);
	}

	void Scene::RenderAllShadow(Ref<Shader> shader)
	{
		for (auto& obj : m_GObjects)
		{
			if (obj->cast_shadow)
				obj->render(shader);
		}

	}

	void Scene::RenderAllFoam(Ref<Shader> shader)
	{
		for (auto& obj : m_GObjects)
		{
			if (obj->cast_shadow)
				obj->render(shader);
		}
		m_skybox->RenderSkybox(m_camera);

	}

	void Scene::RenderAllWitTheirShader()
	{
		for (auto& obj : m_GObjects)
		{
			Ref<Shader> s = obj->shader;
			if (s != nullptr)
				obj->render(s);
		}

		m_skybox->RenderSkybox(m_camera);
	}

	void Scene::checkFrustum()
	{
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

	void Scene::addObjectToScene(Ref<GObject> obj)
	{
		if (obj != nullptr)
		{
			m_root->add_child(obj);
			obj->add_parent(m_root);
			obj->addToScene(colMan);
			obj->scene = this;

			m_GObjects.push_back(obj);
		}
	}

	Ref<GObject> Scene::getObjectByName(const std::string& name)
	{
		return Ref<GObject>();
	}

	std::vector<Ref<GObject>> Scene::getAllObjects()
	{
		return std::vector<Ref<GObject>>();
	}

	std::vector<Ref<GObject>> Scene::getAllObjectsByName(const std::string& name)
	{
		return std::vector<Ref<GObject>>();
	}

	Ref<GObject> Scene::getObjectByID(unsigned int ID)
	{
		return Ref<GObject>();
	}

	std::string Scene::getName()
	{
		return m_name;
	}
}