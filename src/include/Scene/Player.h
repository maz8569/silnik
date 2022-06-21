#pragma once
#include <glm/glm.hpp>
#include <Scene/GComponent.h>
#include<Scene/Box.h>
#include<Scene/GameManager.h>
#include "Utils/InputManager.h"
#include <Audio/AudioManager.h>

namespace GameEngine {
	class Player: public GComponent  
	{
	private:
		float speed = 20;
		float jumpHeight = 30;
		float gravity = -100;
		bool isGrounded = false;
		glm::vec2 currentSpeed = { 0, 0 };
		float jumpPower = 0;

		Ref<GameManager> m_gameManager;

		glm::vec3 lastPosition;

		Ref<InputManager> inputManager;

		Ref<Box> package = nullptr;
		GObject* boat = nullptr;
		
		glm::vec3 proxy;
		glm::vec3 parentProxy;

		void getPackage();
		AudioManager* auman;

		float drownTimer = 0;

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

