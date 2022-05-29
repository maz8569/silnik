#pragma once
#include "Scene/Entity.h"
#include "Physics/RayCasting/MousePicker.h"
#include "typedefs.h"

namespace GameEngine {
	class Courier : public Entity
	{
	private:
		float m_speed = 1.f;
		Ref<MousePicker> m_mousePicker;
		Ref<InputManager> m_inputManager;

	public:
		Courier(Ref<MousePicker> mousePicker, Ref<Model> model, std::shared_ptr<Collision> colMan);
		virtual void render(Ref<Shader> shader);
		virtual void Update(float dt);
		virtual void OnCollisionStay(GObject* other);
	};
}


