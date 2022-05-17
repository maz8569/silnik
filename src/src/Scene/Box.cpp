#include "Scene/Box.h"

namespace GameEngine {



	Box::Box(DeliveryColor color, Ref<Model> model, std::shared_ptr<Collision> colMan)
		: m_color(color), GObject(model, colMan)
	{

	}

	DeliveryColor Box::getDeliveryColor()
	{
		return m_color;
	}

}
