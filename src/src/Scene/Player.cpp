#include "Scene/Player.h"

GameEngine::Player::Player(std::shared_ptr<InputManager> inputManager, Ref<GameManager> gameManager, std::shared_ptr<Model> model, std::shared_ptr<Collision> colMan) 
    : inputManager(inputManager), m_gameManager(gameManager), Entity(model, colMan) 
{
    lastPosition = get_transform().m_position;
}

GameEngine::Player::Player(std::shared_ptr<Model> model, std::shared_ptr<Collision> colMan)
    : Entity(model, colMan)
{
}

void GameEngine::Player::render(Ref<Shader> shader)
{
    Entity::render(shader);
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

    //update_transform(get_parent()->get_transform(), true);

    if (package != nullptr)
    {
        package->get_transform().m_position = get_transform().m_position + glm::vec3(0, 2, 0);
    }

    Entity::Update();
    isGrounded = false;
}

void GameEngine::Player::jump()
{
    jumpPower = jumpHeight;
    isGrounded = false;
}

void GameEngine::Player::reactOnCollision(GObject* other)
{
    
    auto otherAABB = other->getAABB();
    
    if (otherAABB->tag == "water")
    {
        //std::cout << "water";
        get_transform().m_position = lastPosition;
    }

    if (otherAABB->tag == "terrain")
    {
        lastPosition = get_transform().m_position;
    }

    if (otherAABB->tag == "package")
    {
        package = (Box*) other;
        if(package->getDeliveryColor() == DeliveryColor::Blue)
            std::cout << "package";
        package->get_transform().m_scale = { 0.5, 0.5, 0.5 };
    }

    if (otherAABB->tag == "house")
    {
        Box* house = (Box*)other;
        //
        if (package != nullptr)
        {
            if (house->getDeliveryColor() == package->getDeliveryColor())
            {
                package->get_transform().m_position += glm::vec3({ 0, -20, 0 });
                package = nullptr;
                 
                if (m_gameManager != nullptr)
                {
                    m_gameManager->givePackage();
                }
            }
            else
            {
                std::cout << "house";
            }
        }


    }

    auto vec = getAABB()->testDepth(otherAABB);
    
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
    
    //std::cout << vec[0] << " " << vec[1] << " " << vec[2] << "\n";
    switch(i) {
    case 0:
        get_transform().m_position.x -= vec[0];
        break;
    case 1:
        get_transform().m_position.y -= vec[1];
        if(vec[1] < 0)
            isGrounded = true;
        break;
    case 2:
        get_transform().m_position.z -= vec[2];

        break;
    }

    //MoveColliders();
}
