#include "Scene/Entity.h"

GameEngine::Entity::Entity(std::shared_ptr<Model> model) : GObject("")
{
}

void GameEngine::Entity::render(Ref<Shader> shader)
{
	GObject::render(shader);
}

void GameEngine::Entity::Update(float dt)
{
	GObject::Update(dt);
}

void GameEngine::Entity::OnCollisionStay(GObject* other)
{
}
