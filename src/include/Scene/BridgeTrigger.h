#pragma once

#include "GComponent.h"

namespace GameEngine {

	class BridgeTrigger : public GComponent
	{

	private:

		bool isBoatNear = false;

	public:

		Ref<GObject> bridgepart1;
		Ref<GObject> bridgepart2;

		virtual void setParent(GObject* newParent);

		virtual void Update(float dt);

		virtual void OnCollisionEnter(GObject* other);
		virtual void OnCollisionExit(GObject* other);

	};

}
