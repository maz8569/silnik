#include "Scene/Stealing.h"
#include <Scene/GObject.h>

namespace GameEngine {



	Stealing::Stealing()
	{

	}

	void Stealing::Update(float dt)
	{

		if (package != nullptr)
		{
			package->parent->set_local_position(parent->get_transform().m_position + glm::vec3(0, 3, 0));
		}

		parent->set_local_position(boat->get_transform().m_position + glm::vec3(0, 0.5, 0));
	}

	void Stealing::OnCollisionEnter(GObject* other)
	{
		if (other->getAABB()->tag == "package")
		{
			package = other->GetComponent<Box>();
			package->currentHolder = this;
			//if (package->getDeliveryColor() == DeliveryColor::Blue)
			std::cout << "package";
			//package->parent->get_transform().m_scale = { 0.5, 0.5, 0.5 };
		}
	}

	void Stealing::addBoat(Ref<GObject> b)
	{
		boat = b;
	}

	void Stealing::releasePackage()
	{
		package = nullptr;
	}

}