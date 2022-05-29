#include "Scene/Light.h"
#include "glm/gtc/matrix_transform.hpp"

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
		shader->setVec3("dirLight.ambient", dirLightColor * 0.4f);
		shader->setVec3("dirLight.diffuse", dirLightColor * 0.8f);
		shader->setVec3("dirLight.specular", dirLightColor);
	}

	glm::mat4 DirectionalLight::getLightSpaceMatrix(Ref<Camera> camera)
	{
		const auto corners = getFrustumCornersWorldSpace(camera->m_projectionMatrix, camera->m_viewMatrix);
		glm::vec3 center = glm::vec3(0, 0, 0);
		for (const auto& v : corners)
		{
			center += glm::vec3(v);
		}
		center /= corners.size();

		const auto lightView = glm::lookAt(center - lightPos, center, glm::vec3(0.0f, 1.0f, 0.0f));

		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::min();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::min();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::min();
		for (const auto& v : corners)
		{
			const auto trf = lightView * v;
			minX = std::min(minX, trf.x);
			maxX = std::max(maxX, trf.x);
			minY = std::min(minY, trf.y);
			maxY = std::max(maxY, trf.y);
			minZ = std::min(minZ, trf.z);
			maxZ = std::max(maxZ, trf.z);
		}

		// Tune this parameter according to the scene
		constexpr float zMult = 10.0f;
		if (minZ < 0)
		{
			minZ *= zMult;
		}
		else
		{
			minZ /= zMult;
		}
		if (maxZ < 0)
		{
			maxZ /= zMult;
		}
		else
		{
			maxZ *= zMult;
		}

		const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

		return lightProjection * lightView;
	}

	std::vector<glm::vec4> DirectionalLight::getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
	{
		return getFrustumCornersWorldSpace(proj * view);
	}

	std::vector<glm::vec4> DirectionalLight::getFrustumCornersWorldSpace(const glm::mat4& projview)
	{
		const auto inv = glm::inverse(projview);

		std::vector<glm::vec4> frustumCorners;
		for (unsigned int x = 0; x < 2; ++x)
		{
			for (unsigned int y = 0; y < 2; ++y)
			{
				for (unsigned int z = 0; z < 2; ++z)
				{
					const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
					frustumCorners.push_back(pt / pt.w);
				}
			}
		}

		return frustumCorners;
	}

}