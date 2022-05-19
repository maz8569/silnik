#pragma once
#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H
#include <list>
#include "GComponent.h"

namespace GameEngine {

	struct Transform : public GComponent
	{
		Transform() : GComponent(nullptr), m_world_matrix(1.0f) {}
	
		glm::mat4 get_combined_matrix()
		{	
			return glm::translate(glm::mat4(1.0f), m_position) *
					glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(-1, 0, 0)) *
					glm::rotate (glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0, -1, 0)) *
					glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0, 0, -1)) *
					glm::scale(glm::mat4(1.0f), m_scale);
		}

		glm::vec3 getRight() const
		{
			return m_world_matrix[0];
		}


		glm::vec3 getUp() const
		{
			return m_world_matrix[1];
		}

		glm::vec3 getBackward() const
		{
			return m_world_matrix[2];
		}

		glm::vec3 getForward() const
		{
			return -m_world_matrix[2];
		}

		glm::vec3 getGlobalScale() const
		{
			return { glm::length(getRight()), glm::length(getUp()), glm::length(getBackward()) };
		}

		glm::mat4 m_world_matrix;

		glm::vec3 m_position = glm::vec3(0.0f);
		glm::vec3 m_rotation = glm::vec3(0.0f);
		glm::vec3 m_scale = glm::vec3(1.0f);
	};
}
#endif
