#pragma once
#include "GComponent.h"
#include "Box.h"
#include <Utils/InputManager.h>

namespace GameEngine {

	class Dig : public GComponent
	{
	private:

		int m_amount;
		DeliveryColor m_color;

		bool isClose = false;

		Ref<InputManager> m_inputManager;

	public:

		Dig(int amount, DeliveryColor color, Ref<InputManager> inputManager);

		virtual void Update(float dt);

		virtual void OnCollisionEnter(GObject* other);
		virtual void OnCollisionExit(GObject* other);
	};

}
