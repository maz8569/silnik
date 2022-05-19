#pragma once
#include <typedefs.h>
#include <string>

namespace GameEngine {

	class GObject;

	struct GComponent
	{
	private:

		std::uint32_t m_ID;
		GObject* m_parent;

	public:

		GComponent(GObject* parent);

	};

}