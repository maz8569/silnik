#pragma once

#include <iostream>
#include <vector>
#include "EventStruct.h"

namespace GameEngine {

	namespace EventSystem {
		void InitEventSystem(GLFWwindow* window, std::vector<Event>* event_queue);
		void QueueEvent(Event e);
	}

}