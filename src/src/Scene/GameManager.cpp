#include "Scene/GameManager.h"

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

}