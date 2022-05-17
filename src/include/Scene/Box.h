#pragma once

#include "GObject.h"

namespace GameEngine {

	enum DeliveryColor {
		Blue,
		Red,
		Yellow,
		None
	};

	class Box : public GObject
	{
	private:
		DeliveryColor m_color;

	public:

		Box(DeliveryColor color,Ref<Model> model, std::shared_ptr<Collision> colMan);

		DeliveryColor getDeliveryColor();
	};

}