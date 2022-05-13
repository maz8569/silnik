#pragma once

#include <typedefs.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <Texture/Texture.h>
#include <string>

namespace GameEngine {

	class GuiComponent
	{
	private:
		
		Ref<GTexture> m_texture;
		glm::vec2 m_position;
		glm::vec2 m_scale;
		glm::mat4 m_transfrom_matrix;
		bool isDirty = true;



	public:

		static bool windowCh;

		static unsigned int m_scr_width, m_src_height;

		GuiComponent(std::string texture, glm::vec2 position, glm::vec2 scale);
		~GuiComponent();

		glm::mat4 getTransformatiomMatrix();
		glm::mat4 getTransformMatrix();

		static void setScrWidth(unsigned int scr_width);
		static void setScrHeight(unsigned int scr_height);

		Ref<GTexture> getTexture();
	};

}
