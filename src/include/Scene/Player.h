#pragma once
#include <glm/glm.hpp>
#include<Scene/Entity.h>
#include<Scene/Box.h>
#include<Scene/GameManager.h>
#include "Utils/InputManager.h"

namespace GameEngine {
	class Player: public Entity
	{
	private:
		float speed = 20;
		float jumpHeight = 40;
		float gravity = -50;
		bool isGrounded = false;
		glm::vec2 currentSpeed = { 0, 0 };
		float jumpPower = 0;

		Ref<GameManager> m_gameManager;

		glm::vec3 lastPosition;

		std::shared_ptr<InputManager> inputManager;

		Box* package = nullptr;
		
	public:
		Player(std::shared_ptr<InputManager> inputManager, Ref<GameManager> gameManager, std::shared_ptr<Model> model, std::shared_ptr<Collision> colMan);
		Player( std::shared_ptr<Model> model, std::shared_ptr<Collision> colMan);

		virtual void render(Ref<Shader> shader);
		virtual void Update(float dt);
		void jump();
		virtual void OnCollisionStay(GObject* other);
	};
}

