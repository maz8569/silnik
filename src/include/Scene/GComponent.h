#pragma once

namespace GameEngine {

	class GObject;

	struct GComponent
	{

		GObject* parent;
		unsigned int ID;

		GComponent(GObject* obj);

	};

}