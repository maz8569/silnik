#include "Physics/Collisions/Collision2D.h"

namespace GameEngine {


	void Collision2D::addCollision(Ref<GuiComponent> c)
	{
		c->onCollisionEnter(m_mouseCursor->mousePos);
		m_collisions.push_back(c);
	}

	void Collision2D::removeCollision(Ref<GuiComponent> c)
	{
		c->onCollisionExit(m_mouseCursor->mousePos);
		std::remove(m_collisions.begin(), m_collisions.end(), c);
	}

	Collision2D::Collision2D(Ref<MouseCursor> mouseCursor)
		: m_mouseCursor(mouseCursor)
	{
	}

	bool Collision2D::testPointAABB(Ref<GuiComponent> c, glm::vec2 pos)
	{
		glm::vec2 min = c->getPosition() - c->getScale();
		glm::vec2 max = c->getPosition() + c->getScale();

		if (pos.x < max.x && pos.y < max.y && pos.x > min.x && pos.y > min.y)
		{
			return true;
		}

		return false;
	}

	void Collision2D::CollisionCheck()
	{
		if (m_collisions.size() > 0)
		{
			for (auto c : m_collisions) {
				if (c != nullptr)
				{
					if (testPointAABB(c, m_mouseCursor->mousePos))
					{
						c->onCollisionStay(m_mouseCursor->mousePos);
					}
					else
					{
						removeCollision(c);
					}
				}

			}
		}

		if (m_2DAABB.size() > 0) {
			for (auto c : m_2DAABB) {
				if (!std::count(m_collisions.begin(), m_collisions.end(), c))
				{
					if (testPointAABB(c, m_mouseCursor->mousePos))
					{
						addCollision(c);
					}
				}
			}
		}

	}

	void Collision2D::addGuiComponent(Ref<GuiComponent> c)
	{
		m_2DAABB.push_back(c);
	}

}