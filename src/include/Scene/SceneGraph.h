#pragma once
#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GameEngine {

	struct Transform
	{
		Transform();
	
		glm::mat4 get_combined_matrix();
		glm::vec3 getRight() const;
		glm::vec3 getUp() const;
		glm::vec3 getBackward() const;
		glm::vec3 getForward() const;
		glm::vec3 getGlobalScale() const;

		glm::mat4 m_world_matrix;

		glm::vec3 m_position = glm::vec3(0.0f);
		glm::vec3 m_rotation = glm::vec3(0.0f);
		glm::vec3 m_scale = glm::vec3(1.0f);
	};
}
