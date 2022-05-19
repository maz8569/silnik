#include "Scene/Courier.h"
#include "Physics/Collisions/Collision.h"

GameEngine::Courier::Courier(Ref<MousePicker> mousePicker, Ref<Model> model, std::shared_ptr<Collision> colMan) 
	: Entity(model, colMan), m_mousePicker(mousePicker)
{
	m_inputManager = mousePicker->getInputManager();
	Entity::Update();
}

void GameEngine::Courier::render(Ref<Shader> shader)
{
	Entity::render(shader);
}

void GameEngine::Courier::Update()
{
	//if (get_transform().m_position.y > 10 || get_transform().m_position.y < -10)
		//speed *= -1;
	//get_transform().m_position.y += 0.01 * speed;
	//update_transform(get_parent()->get_transform(), true);
	if (m_inputManager->m_isRclicked)
	{
		glm::vec3 start = m_mousePicker->getCameraPos();
		glm::vec3 dir = m_mousePicker->getCurrentRay();
		//std::cout << dir.x << " " << dir.y << " " << dir.z << " " << std::endl;

		float time;
		glm::vec3 end;
		
		if (m_colman->IntersectRayAABB(start, dir, time, end))
		{
			get_transform().m_position = end;
		}
		update_transform(get_parent()->get_transform(), true);



	}


	Entity::Update();
}

void GameEngine::Courier::reactOnCollision(GObject* other)
{
	//std::cout << "end";
}
