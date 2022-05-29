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
		}
	}
	void BridgeTrigger::OnCollisionExit(GObject* other)
	{
		if (other->getAABB()->tag == "boat")
		{
			isBoatNear = false;
			bridgepart2->set_local_rotationZ(0);
			bridgepart1->set_local_rotationZ(0);
		}
	}
}