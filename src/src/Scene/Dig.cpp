#include "Scene/Dig.h"
#include "Scene/GObject.h"
#include <Scene/GameManager.h>
#include <Scene/Scene.h>

namespace GameEngine {
	
	Dig::Dig(int amount, DeliveryColor color, Ref<InputManager> inputManager)
		: m_amount(amount), m_color(color), m_inputManager(inputManager)
	{

	}

	void Dig::Update(float dt)
	{

		if (m_amount == 0 && m_color != DeliveryColor::None)
		{
			parent->scene->gameManager->makePackageAt(m_color, parent->get_transform().m_position + glm::vec3(0, 3, 0), parent->scene);
			parent->setAABBoffsets({ 0, -10, 0 });
			m_color = None;
		}

		if (m_inputManager->m_isLclicked && isClose)
		{
			if (m_amount > 0)
			{
				float y = parent->get_transform().m_scale.y - 0.05;
				parent->set_local_scale({ 1, y, 1 });
				m_amount--;
			}
		}


	}

	void Dig::OnCollisionEnter(GObject* other)
	{
		if (other->getAABB()->tag == "player")
		{
			isClose = true;
		}
	}

	void Dig::OnCollisionExit(GObject* other)
	{
		if (other->getAABB()->tag == "player")
		{
			isClose = false;
		}
	}

}