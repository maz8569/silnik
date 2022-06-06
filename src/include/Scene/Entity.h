#pragma once
#include "Scene/GObject.h"

namespace GameEngine {
	class Entity: public GObject
	{
	private:

	public:
		Entity(std::shared_ptr<Model> model);
		virtual void render(Ref<Shader> shader);
		virtual void Update(float dt);
		virtual void OnCollisionStay(GObject* other);
	};
}

