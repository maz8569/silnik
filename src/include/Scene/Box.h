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

		GComponent* currentHolder = nullptr;

		Box(DeliveryColor color);

		DeliveryColor getDeliveryColor();
	};

}