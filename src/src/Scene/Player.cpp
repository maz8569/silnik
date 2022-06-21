#include "Scene/Player.h"
#include <Scene/Stealing.h>
#include <Scene/Boat.h>

namespace GameEngine {

    void Player::getPackage()
    {
        package->currentHolder = this;
    }

    Player::Player(std::shared_ptr<InputManager> inputManager, Ref<GameManager> gameManager)
        : inputManager(inputManager), m_gameManager(gameManager), GComponent()
    {
        auman = AudioManager::getInstance();
    }

void Player::setParent(GObject* newParent)
{
    GComponent::setParent(newParent);
    lastPosition = parent->get_transform().m_position;

}

void Player::Update(float dt)
{
    currentSpeed.x = speed * inputManager->getHorizontal();

    currentSpeed.y = -speed * inputManager->getVertical();

    if ((currentSpeed.x != 0) || (currentSpeed.y != 0))
    {
        float yRot = atan2(-currentSpeed.x, currentSpeed.y);

        parent->set_local_rotationY(glm::degrees(yRot));
    }

    if (drownTimer <= 0)
    {
        //currentSpeed = glm::normalize(currentSpeed);
        parent->get_transform().m_position.x += currentSpeed.x * 0.005;

        parent->get_transform().m_position.z += currentSpeed.y * 0.005;

        if (boat != nullptr)
        {
            parent->get_transform().m_position = boat->get_transform().m_position + glm::vec3(0, 0.4, 0);
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
    }
    else
    {
        drownTimer -= dt;
        parent->get_transform().m_position.y += jumpPower * 0.005;
        if (drownTimer <= 0)
        {
            parent->get_transform().m_position = lastPosition;// +glm::vec3(0, 0.1, 0);
        }
    }

    if (package != nullptr)
    {
        package->parent->get_transform().m_position = parent->get_transform().m_position + glm::vec3(0, 2, 0);
    }

    isGrounded = false;
}

void Player::jump()
{
    jumpPower = jumpHeight;
    isGrounded = false;
    boat = nullptr;
    auman->playMonoSound("jump");
}

void Player::OnCollisionEnter(GObject* other)
{
    if (other->getAABB()->tag == "water")
    {
        //std::cout << "water";
        drownTimer = 3;
        jumpPower = -2;
    }
    if (other->getAABB()->tag == "boat")
    {
        boat = other;
    }
}

void Player::OnCollisionStay(GObject* other)
{
    
    auto otherAABB = other->getAABB();
    auto tag = otherAABB->tag;
    

    
    if (tag == "Tboat" && boat != nullptr && package == nullptr)
    {
        Ref<Stealing> stealComp = other->GetComponent<Stealing>();
        if (stealComp != nullptr)
        {
            package = stealComp->getPackage();
            stealComp->releasePackage();
            if (package != nullptr)
                getPackage();
        }
    }

    
    if (tag == "boat" && jumpPower <-2)
    {
        boat = other;
    }
    

    if (tag == "Tbridge" || tag == "Tboat" || tag == "boat")
    {
        return;
    }



    if (tag == "package")
    {
        package = other->GetComponent<Box>();
        if (package->currentHolder != nullptr)
        {
            Stealing* stealing = (Stealing*)package->currentHolder;
            stealing->releasePackage();
            getPackage();
        }
        if(package->getDeliveryColor() == DeliveryColor::Blue)
            std::cout << "package";
        package->parent->get_transform().m_scale = { 0.5, 0.5, 0.5 };
    }

    if (tag == "house")
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

    if (tag == "terrain" && isGrounded)
    {
        proxy = other->get_transform().m_position;
        parentProxy = parent->get_transform().m_position;
        proxy.y = parentProxy.y;
        lastPosition = 0.5f * parentProxy + 0.5f * proxy;
        lastPosition += glm::vec3(0, 0.5, 0);
    }

    parent->MoveColliders();
}

    void Player::OnCollisionExit(GObject* other)
    {
    }

}