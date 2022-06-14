#pragma once

#include "GComponent.h"

namespace GameEngine {

	class Island : public GComponent
	{

	public:

		bool isFree;

		Island(bool taken);

	};

}