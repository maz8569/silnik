#pragma once

#include <typedefs.h>
#include <Shaders/Shader.h>
#include <Utils/VertexArray.h>
#include <Utils/VertexBuffer.h>
#include "GuiComponent.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <Scene/Camera.h>
#include <vector>

namespace GameEngine {

	class GUIRenderer
	{
	private:
		VertexArray m_vao;
		VertexBuffer m_vbo;

		Ref<Shader> m_guiShader;
		Ref<Camera> m_camera;

	public:

		GUIRenderer(Ref<Camera> camera);
		~GUIRenderer();

		void Render(std::vector<Ref<GuiComponent>> toDraw);

	};

}
