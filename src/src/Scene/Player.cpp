#include "Scene/Player.h"

namespace GameEngine { 

    Player::Player(std::shared_ptr<InputManager> inputManager, Ref<GameManager> gameManager) 
        : inputManager(inputManager), m_gameManager(gameManager), GComponent()
    {
    }

void GameEngine::Player::setParent(GObject* newParent)
{
    GComponent::setParent(newParent);
    lastPosition = parent->get_transform().m_position;

}

void GameEngine::Player::Update(float dt)
{
    currentSpeed.x = speed * inputManager->getHorizontal();

    currentSpeed.y = -speed * inputManager->getVertical();

    //currentSpeed = glm::normalize(currentSpeed);
    parent->get_transform().m_position.x += currentSpeed.x * 0.005;

    parent->get_transform().m_position.z += currentSpeed.y * 0.005;

    if (boat != nullptr)
    {
        parent->get_transform().m_position = boat->get_transform().m_position + glm::vec3(0, 3, 0);
        isGrounded = true;
    }

    jumpPower += gravity / 60;

    if (isGrounded)
    {
        jumpPower = 0;
    }

    parent->get_transform().m_position.y += jumpPower * 0.005;


    if (inputManager->getJump() && isGrounded) {
        jump();
    }

    //update_transform(get_parent()->get_transform(), true);

    if (package != nullptr)
    {
        package->parent->get_transform().m_position = parent->get_transform().m_position + glm::vec3(0, 2, 0);
    }

    isGrounded = false;
}

void GameEngine::Player::jump()
{
    jumpPower = jumpHeight;
    isGrounded = false;
    boat = nullptr;
}

void GameEngine::Player::OnCollisionEnter(GObject* other)
{
    if (other->getAABB()->tag == "boat")
    {
        boat = other;
    }
}

void GameEngine::Player::OnCollisionStay(GObject* other)
{
    
    auto otherAABB = other->getAABB();
    
    if (otherAABB->tag == "water")
    {
        //std::cout << "water";
        parent->get_transform().m_position = lastPosition;
    }
    

    if (otherAABB->tag == "Tbridge")
    {
        return;
    }

    if (otherAABB->tag == "terrain")
    {
        lastPosition = parent->get_transform().m_position;
    }

    if (otherAABB->tag == "package")
    {
        package = other->GetComponent<Box>();
        if(package->getDeliveryColor() == DeliveryColor::Blue)
            std::cout << "package";
        package->parent->get_transform().m_scale = { 0.5, 0.5, 0.5 };
    }

    if (otherAABB->tag == "house")
    {
        Ref<Box> house = other->GetComponent<Box>();
        //
        if (package != nullptr)
        {
            if (house->getDeliveryColor() == package->getDeliveryColor())
            {
                package->parent->get_transform().m_position += glm::vec3({ 0, -20, 0 });
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



    auto vec = parent->getAABB()->testDepth(otherAABB);
    
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
        parent->get_transform().m_position.x -= vec[0];
        break;
    case 1:
        parent->get_transform().m_position.y -= vec[1];
        if(vec[1] < 0)
            isGrounded = true;
        break;
    case 2:
        parent->get_transform().m_position.z -= vec[2];

        break;
    }

    //MoveColliders();
}

    void Player::OnCollisionExit(GObject* other)
    {
    }

}