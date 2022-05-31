#pragma once

#include "GObject.h"
#include <Texture/Texture.h>
#include <GUI/GuiComponent.h>

namespace GameEngine {

	enum GState {
		Playing,
		Win,
		Lose
	};

	class GameManager : public GObject
	{
	private:
		int m_packageCount;

		std::vector<Ref<GTexture>> m_textures;

		Ref<GuiComponent> m_GuiNumber;

		GState m_win;

		float time = 60.0f;

	public:

		GameManager(int packageCount, Ref<GuiComponent> GuiNumber);

		void init();

		virtual void Update(float dt);

		float getTime();
		void setTime(float t);

		void addTexture(Ref<GTexture> texture);
		void givePackage();

		GState isWin();
	};

}