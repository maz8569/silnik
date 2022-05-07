#include "Scene/Entity.h"

GameEngine::Entity::Entity(std::shared_ptr<Model> model, std::shared_ptr<Collision> colMan) : GObject(model, colMan)
{
}

void GameEngine::Entity::render(Ref<Shader> shader)
{
	GObject::render(shader);
}

void GameEngine::Entity::Update()
{
	GObject::Update();
}

void GameEngine::Entity::reactOnCollision(GObject* other)
{
}
