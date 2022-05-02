#include "Scene/Light.h"

namespace GameEngine {

	DirectionalLight::DirectionalLight(glm::vec3 direction)
	{
		lightPos = direction;
	}

	DirectionalLight::~DirectionalLight()
	{
	}

	void GameEngine::DirectionalLight::activate_lights(Ref<Shader> shader, Ref<Camera> camera)
	{
		shader->setFloat("shininess", shinyyy);
		shader->setVec3("viewPos", camera->Position);

		glm::vec3 dirLightColor = { 1, 1, 0.9f };
		shader->setVec3("dirLight.direction", lightPos);
		shader->setVec3("dirLight.ambient", dirLightColor * 0.2f);
		shader->setVec3("dirLight.diffuse", dirLightColor * 0.5f);
		shader->setVec3("dirLight.specular", dirLightColor * 0.5f);
	}
}