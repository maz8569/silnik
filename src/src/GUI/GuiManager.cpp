#include "GUI/GuiManager.h"

namespace GameEngine {


	GuiManager::GuiManager(Ref<MouseCursor> mouseCursor, Ref<Camera> camera)
	{
		colMan2D = std::make_shared<Collision2D>(mouseCursor);
		guiRenderer = CreateRef<GUIRenderer>(camera);
	}

	Ref<GuiComponent> GuiManager::addComponent(std::string texture, glm::vec2 pos, glm::vec2 scale, float rotation)
	{
		Ref<GuiComponent> comp = CreateRef<GuiComponent>(texture, pos, scale, rotation);

		toDraw.push_back(comp);
		colMan2D->addGuiComponent(comp);

		return comp;
	}

	Ref<Slider> GuiManager::addSlider(float minValue, float maxValue, float* defaultValue, std::string texture, glm::vec2 pos, glm::vec2 scale, float rotation)
	{
		Ref<GuiComponent> comp = CreateRef<Slider>(minValue, maxValue, defaultValue, texture, pos, scale);

		toDraw.push_back(comp);
		colMan2D->addGuiComponent(comp);

		Ref<Slider> slider = Cast<Slider>(comp);

		return slider;
	}

	void GuiManager::removeComponent(Ref<GuiComponent> comp)
	{

	}

	void GuiManager::Update()
	{
		colMan2D->CollisionCheck();
		if (toDraw.size() > 0)
		{
			for (auto c : toDraw)
			{
				c->Update();
			}
		}
	}

	void GuiManager::Click()
	{
		if (toDraw.size() > 0)
		{
			for (auto c : toDraw)
			{
				c->onClick();
			}
		}
	}

	void GuiManager::stopClick()
	{
		if (toDraw.size() > 0)
		{
			for (auto c : toDraw)
			{
				c->stopClick();
			}
		}
	}

	void GuiManager::Render()
	{
		guiRenderer->Render(toDraw);
	}

}