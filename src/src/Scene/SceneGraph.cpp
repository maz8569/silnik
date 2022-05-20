#include <Scene/SceneGraph.h>

namespace GameEngine{

	Transform::Transform()
	: m_world_matrix(1.0f) {}

	glm::mat4 Transform::get_combined_matrix()
	{
		return glm::translate(glm::mat4(1.0f), m_position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(-1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0, -1, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0, 0, -1)) *
			glm::scale(glm::mat4(1.0f), m_scale);
	}

	glm::vec3 Transform::getRight() const
	{
		return m_world_matrix[0];
	}

	glm::vec3 Transform::getUp() const
	{
		return m_world_matrix[1];
	}

	glm::vec3 Transform::getBackward() const
	{
		return m_world_matrix[2];
	}

	glm::vec3 Transform::getForward() const
	{
		return -m_world_matrix[2];
	}

	glm::vec3 Transform::getGlobalScale() const
	{
		return { glm::length(getRight()), glm::length(getUp()), glm::length(getBackward()) };
	}



}


