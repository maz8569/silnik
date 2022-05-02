#pragma once

#include <glm/glm.hpp>
#include <typedefs.h>
#include <Shaders/Shader.h>
#include "Camera.h"

namespace GameEngine {

	class DirectionalLight
	{
	public:
		int shinyyy = 128;

		float p_const = 1.0f;
		float p_lin = 0.09f;
		float p_qua = 0.032f;
		float p_ambient = 0.3f;
		float p_diffuse = 1.0f;
		float p_specular = 1.0f;

		glm::vec3 lightPos;

		DirectionalLight(glm::vec3 direction);
		~DirectionalLight();

		void activate_lights(Ref<Shader> shader, Ref<Camera> camera);
	};

}