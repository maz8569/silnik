#pragma once

#include <typedefs.h>
#include <glm/glm.hpp>
#include <Texture/BasicTexture.h>
#include <string>

namespace GameEngine {

	class GuiComponent
	{
	private:
		
		Ref<BasicTexture> m_texture;
		glm::vec2 m_position;
		glm::vec2 m_scale;

	public:

		GuiComponent(std::string texture, glm::vec2 position, glm::vec2 scale);
		~GuiComponent();

		Ref<BasicTexture> getTexture();
	};

}
