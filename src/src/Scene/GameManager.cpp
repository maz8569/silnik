#include "Scene/GameManager.h"
#include <Utils/ResourceManager.h>
#include <Scene/Scene.h>

namespace GameEngine {

	GameManager::GameManager(int packageCount, Ref<GuiComponent> GuiNumber)
		: GObject(""), m_packageCount(packageCount), m_GuiNumber(GuiNumber)
	{
		m_win = GState::Playing;
	}

	void GameManager::init()
	{
		m_GuiNumber->setTexture(m_textures[m_packageCount]);

	}

	void GameManager::Update(float dt)
	{
		if (time <= 0)
		{
			m_win = GState::Lose;
			time = 0;
		}
	}

	float GameManager::getTime()
	{
		return time;
	}

	void GameManager::setTime(float t)
	{
		time = t;
	}

	void GameManager::addTexture(Ref<GTexture> texture)
	{
		m_textures.push_back(texture);
	}

	void GameManager::givePackage()
	{
		m_packageCount -= 1;

		init();

		if (m_packageCount == 0)
		{
			m_win = GState::Win;
		}

	}

	GState GameManager::isWin()
	{
		return m_win;
	}

	void GameManager::makePackageAt(DeliveryColor color, glm::vec3 position, Scene* scene)
	{
		Ref<GObject> paczka = CreateRef<GObject>("package");
		Ref<Box> boxp = CreateRef<Box>(color);
		paczka->addComponent(boxp);
		paczka->shader = ResourceManager::getShader("ourShader");
		paczka->set_local_position(position);
		paczka->getAABB()->setStatic(true);
		//paczka->set_render_AABB(true);
		paczka->set_tag("package");

		scene->addObjectToScene(paczka);
	}

	void GameManager::setState(GState st)
	{
		m_win = st;
	}

}