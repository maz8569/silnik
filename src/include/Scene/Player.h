#pragma once
#include <glm/glm.hpp>
#include <Scene/GComponent.h>
#include<Scene/Box.h>
#include<Scene/GameManager.h>
#include "Utils/InputManager.h"

namespace GameEngine {
	class Player: public GComponent  
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

		Ref<InputManager> inputManager;

		Ref<Box> package = nullptr;
		GObject* boat = nullptr;
		
	public:
		Player(std::shared_ptr<InputManager> inputManager, Ref<GameManager> gameManager);


		virtual void setParent(GObject* newParent);

		virtual void Update(float dt);
		void jump();
		virtual void OnCollisionEnter(GObject* other);
		virtual void OnCollisionStay(GObject* other);
		virtual void OnCollisionExit(GObject* other);
	};
}

