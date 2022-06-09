#pragma once

#include "GComponent.h"
#include "Box.h"


namespace GameEngine {

	class Stealing : public GComponent
	{
	private:

		Ref<Box> package = nullptr;
		Ref<GObject> boat = nullptr;

	public:

		Stealing();

		virtual void Update(float dt);
		virtual void OnCollisionEnter(GObject* other);

		void addBoat(Ref<GObject> b);
		void releasePackage();
	};

}