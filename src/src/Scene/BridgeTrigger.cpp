#include "Scene/BridgeTrigger.h"

#include <Scene/GObject.h>

namespace GameEngine {
	void BridgeTrigger::setParent(GObject* newParent)
	{
		GComponent::setParent(newParent);
	}
	void BridgeTrigger::Update(float dt)
	{
	}

	void BridgeTrigger::OnCollisionEnter(GObject* other)
	{
		if (other->getAABB()->tag == "boat")
		{
			isBoatNear = true;
			bridgepart2->set_local_rotationZ(-90);
			bridgepart1->set_local_rotationZ(-90);

			if(bridgepart2->getAABB()->tag == "dB" || bridgepart2->getAABB()->tag == "uB")
			{ 
				bridgepart2->setAABBrotation(Degrees::D90, Axis::X);
				bridgepart1->setAABBrotation(Degrees::D90, Axis::X);
			}
			else {
				bridgepart2->setAABBrotation(Degrees::D270, Axis::Z);
				bridgepart1->setAABBrotation(Degrees::D270, Axis::Z);
			}

		}
	}
	void BridgeTrigger::OnCollisionExit(GObject* other)
	{
		if (other->getAABB()->tag == "boat")
		{
			isBoatNear = false;
			bridgepart2->set_local_rotationZ(0);
			bridgepart1->set_local_rotationZ(0);

			if (bridgepart2->getAABB()->tag == "dB" || bridgepart2->getAABB()->tag == "uB")
			{ 
				bridgepart2->setAABBrotation(Degrees::D270, Axis::X);
				bridgepart1->setAABBrotation(Degrees::D270, Axis::X);
			}
			else
			{
				bridgepart2->setAABBrotation(Degrees::D90, Axis::Z);
				bridgepart1->setAABBrotation(Degrees::D90, Axis::Z);
			}

		}
	}
}