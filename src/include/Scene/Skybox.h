#pragma once

#include <Utils/VertexArray.h>
#include <Utils/VertexBuffer.h>
#include <Shaders/Shader.h>
#include <Texture/CubemapTexture.h>
#include <typedefs.h>
#include <Scene/Camera.h>

namespace GameEngine {

	class Skybox
	{
	public:
		Skybox();
		void RenderSkybox(Ref<Camera> camera);

	private:
		VertexArray m_VAO;
		VertexBuffer m_VBO;
		Shader m_skyboxShader;
		CubemapTexture m_cubeMap;
	};

}