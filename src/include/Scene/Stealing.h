#pragma once

#include "GComponent.h"
#include "Box.h"
#include <vector>
#include <algorithm>
#include <random>

namespace GameEngine {

	class Stealing : public GComponent
	{
	private:

		Ref<Box> package = nullptr;
		Ref<GObject> boat = nullptr;

		unsigned int islandsTreshold = 0;
		std::vector<bool> random;
		std::default_random_engine rng;

		bool randPlace();
		int canTakePackage = 0;

	public:

		Stealing(unsigned int treshold);

		virtual void Update(float dt);
		virtual void OnCollisionEnter(GObject* other);

		void addBoat(Ref<GObject> b);
		void releasePackage();

		Ref<Box> getPackage();

		void resetRandom();
		void placePackage(GObject* other);
	};

}