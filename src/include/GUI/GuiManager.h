#pragma once

#include "GUIRenderer.h"
#include <Physics/Collisions/Collision2D.h>

namespace GameEngine {

	class GuiManager
	{
	private:
		std::vector<Ref<GuiComponent>> toDraw;

		Ref<Collision2D> colMan2D = nullptr;

		Ref<GUIRenderer> guiRenderer = nullptr;

	public:

		GuiManager(Ref<MouseCursor> mouseCursor, Ref<Camera> camera);

		Ref <GuiComponent> addComponent(std::string texture, glm::vec2 pos, glm::vec2 scale, float rotation = 0);
		void removeComponent(Ref <GuiComponent> comp);

		void Update();

		void Click();

		void Render();
	};

}