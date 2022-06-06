#pragma once

#include "GComponent.h"

namespace GameEngine {

	enum DeliveryColor {
		Blue,
		Red,
		Yellow,
		None
	};

	class Box : public GComponent
	{
	private:
		DeliveryColor m_color;

	public:

		Box(DeliveryColor color);

		DeliveryColor getDeliveryColor();
	};

}