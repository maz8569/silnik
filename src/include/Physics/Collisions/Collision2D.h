#pragma once

#include <GUI/GuiComponent.h>
#include <GUI/MouseCursor.h>
#include <vector>
#include <typedefs.h>

namespace GameEngine {

	class Collision2D
	{
	private:
		std::vector<Ref<GuiComponent>> m_2DAABB;
		std::vector< Ref<GuiComponent>> m_collisions;

		int i = 0;

		Ref<MouseCursor> m_mouseCursor;

		void addCollision(Ref<GuiComponent> c);
		void removeCollision(Ref<GuiComponent> c);

	public:
		Collision2D(Ref<MouseCursor> mouseCursor);

		bool testPointAABB(Ref<GuiComponent> c, glm::vec2 pos);
		void CollisionCheck();

		void addGuiComponent(Ref<GuiComponent> c);

	};

}