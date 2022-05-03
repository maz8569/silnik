#include "Scene/Player.h"

GameEngine::Player::Player(std::shared_ptr<InputManager> inputManager, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Collision> colMan) : inputManager(inputManager), Entity(model, shader, colMan) {}

GameEngine::Player::Player(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Collision> colMan)
    : Entity(model, shader, colMan)
{
}

void GameEngine::Player::render()
{
    Entity::render();
}

void GameEngine::Player::Update()
{
    currentSpeed.x = speed * inputManager->getHorizontal();

    currentSpeed.y = -speed * inputManager->getVertical();

    //currentSpeed = glm::normalize(currentSpeed);
    get_transform().m_position.x += currentSpeed.x * 0.005;

    get_transform().m_position.z += currentSpeed.y * 0.005;

    jumpPower += gravity / 60;

    if (isGrounded)
    {
        jumpPower = 0;
        isGrounded = true;
    }

    get_transform().m_position.y += jumpPower * 0.005;


    if (inputManager->getJump() && isGrounded) {
        jump();
    }

    update(get_parent()->get_transform(), true);

    Entity::Update();
}

void GameEngine::Player::jump()
{
    jumpPower = jumpHeight;
    isGrounded = false;
}

void GameEngine::Player::reactOnCollision(GObject* other)
{
    //get_transform().m_position.y -= other->getAABB()->extents[0] * 0.5;
    //get_transform().m_position.x -= currentSpeed.x * 0.005;
    //get_transform().m_position.y -= currentSpeed.y * 0.005;
    
    
    auto vec = getAABB()->testDepth(other->getAABB());
    
    int i = 2;
    
    if (abs(vec[0]) >= abs(vec[1]))
    {
        if (abs(vec[1]) >= abs(vec[2]))
        {
            i = 2;
        }
        else
        {
            i = 1;
        }
    }
    else
    {
        if (abs(vec[0]) >= abs(vec[2]))
        {
            i = 2;
        }
        else
        {
            i = 0;
        }
    }
    
    std::cout << vec[0] << " " << vec[1] << " " << vec[2] << "\n";
    switch(i) {
    case 0:
        get_transform().m_position.x -= vec[0];
        break;
    case 1:
        get_transform().m_position.y -= vec[1];
        isGrounded = true;
        break;
    case 2:
        get_transform().m_position.z -= vec[2];

        break;
    }

    //MoveColliders();
}
