#include "Scene/Stealing.h"
#include <Scene/GObject.h>
#include <chrono>
#include <Scene/Island.h>

namespace GameEngine {


	Stealing::Stealing(unsigned int treshold)
		: islandsTreshold(treshold)
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		rng = std::default_random_engine{seed};
		resetRandom();
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
			if (canTakePackage <= 0)
			{
				package = other->GetComponent<Box>();
				package->currentHolder = this;
				//if (package->getDeliveryColor() == DeliveryColor::Blue)
				std::cout << "package";
				//package->parent->get_transform().m_scale = { 0.5, 0.5, 0.5 };
			}

		}

		if (other->getAABB()->tag == "terrain")
		{
			canTakePackage--;

			if((package != nullptr) && (other->GetComponent<Island>()->isFree))
				placePackage(other);

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

	void Stealing::resetRandom()
	{
		random.clear();
		random.resize(islandsTreshold - 1, false);
		random.push_back(true);

		std::shuffle(std::begin(random), std::end(random), rng);

		std::cout << "\n";
		for (int i = 0; i < random.size(); i++)
		{
			std::cout << " "<< random[i] << " ";
		}
		std::cout << "\n";

	}

	bool Stealing::randPlace()
	{
		std::cout << "terr";

		bool d = random[0];
		random.erase(random.begin());

		std::cout << " " << d << "\n";

		if (d)
		{
			resetRandom();
		}

		return d;
	}

	void Stealing::placePackage(GObject* other)
	{
		if (randPlace())
		{
			package->parent->set_local_position(other->get_transform().m_position + glm::vec3(0, 3, 0));
			releasePackage();
			canTakePackage = islandsTreshold/2;
		}
	}

}