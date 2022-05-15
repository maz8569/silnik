#pragma once

#include <typedefs.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <Texture/Texture.h>
#include <string>
#include <iostream>

namespace GameEngine {

	class GuiComponent
	{
	private:
		
		Ref<GTexture> m_texture;
		glm::vec2 m_position;
		glm::vec2 m_scale;
		glm::mat4 m_transfrom_matrix;
		bool isDirty = true;
		float m_rotation = 0;

		bool isHovered = false;

		glm::vec3 color = { 1, 1, 1 };
		glm::vec3 highlightColor = { 1, 0, 0 };
		float opacity = 0;

	public:

		static bool windowCh;

		static unsigned int m_scr_width, m_src_height;

		GuiComponent(std::string texture, glm::vec2 position, glm::vec2 scale, float rotation = 0);
		~GuiComponent();

		glm::mat4 getTransformatiomMatrix();
		glm::mat4 getTransformMatrix();

		static void setScrWidth(unsigned int scr_width);
		static void setScrHeight(unsigned int scr_height);

		glm::vec3& getColor();
		glm::vec3& getHighLightColor();
		float& getOpacity();

		glm::vec2 getPosition();
		glm::vec2 getScale();

		void onCollisionEnter(glm::vec2 pos);
		void onCollisionStay(glm::vec2 pos);
		void onCollisionExit(glm::vec2 pos);

		void onClick();

		Ref<GTexture> getTexture();
	};

}
