#include "Scene/Box.h"

namespace GameEngine {



	Box::Box(DeliveryColor color)
		: m_color(color), GComponent()
	{

	}

	DeliveryColor Box::getDeliveryColor()
	{
		return m_color;
	}

}
