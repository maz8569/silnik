#pragma once
#ifndef GOBJECT_CLASS_H
#define GOBJECT_CLASS_H

#include <memory>
#include <vector>
#include "Rendering/Model.h"
#include "Scene/SceneGraph.h"
#include <Physics/Collisions/AABB.h>

namespace GameEngine {
	class Collision;

	enum class Degrees
	{
		D90,
		D180,
		D270
	};

	enum class Axis
	{
		X,
		Y,
		Z
	};

	class GObject
	{
	public:

		void add_child(Ref<GObject> child)
		{
			m_children.push_back(child);
		}

		void add_parent(Ref<GObject> parent)
		{
			m_parent = parent;
		}

		Ref<GObject> get_parent()
		{
			return m_parent;
		}

		void update_transform(const Transform& parent_transform, bool dirty)
		{
			dirty |= m_dirty;

			if (dirty)
			{
				m_transform.m_world_matrix = m_transform.get_combined_matrix();
				m_transform.m_world_matrix = parent_transform.m_world_matrix * m_transform.m_world_matrix;
				m_dirty = false;
			}

			for (uint32_t i = 0; i < m_children.size(); ++i)
			{
				m_children[i]->update_transform(m_transform, dirty);
			}
		}

		Transform& get_transform()
		{
			return m_transform;
		}

		void set_local_position(const glm::vec3& newPosition)
		{
			m_transform.m_position = newPosition;
			m_dirty = true;
		}

		void set_local_rotation(const glm::vec3& newRotation)
		{
			m_transform.m_rotation = newRotation;
			m_dirty = true;
		}

		void set_local_scale(glm::vec3 newScale)
		{
			m_transform.m_scale = newScale;
			m_dirty = true;
		}

		std::vector<Ref<GObject>> get_children()
		{
			return m_children;
		}

	private:
		std::vector<Ref<GObject>> m_children;
		Ref<GObject> m_parent;
		Transform m_transform;
		bool m_dirty;

	private:

		Ref<Model> m_model;
		Ref<AABB> m_aabb;
		glm::vec3 m_color;

		bool render_AABB = false;
		unsigned int VBO, VAO;
		glm::vec3 offset;
		std::string m_name;
		std::string m_tag;
		unsigned int ID;
		static unsigned int ms_uiNextID;

		void rotateAABBZ(Degrees deg);

	public:
		Ref<Collision> m_colman;

		GObject();
		explicit GObject(Ref<Model> model, std::shared_ptr<Collision> colMan);
		~GObject();

		void set_tag(std::string newTag);
		void set_color(glm::vec3 c);

		glm::vec3 get_color();

		void setModel(std::shared_ptr<Model> model);
		Ref<AABB> getAABB() const;

		virtual void render(Ref<Shader> shader);
		virtual void Update();

		virtual void reactOnCollision(GObject* other);

		void MoveColliders();

		void set_render_AABB(bool set);

		void recalculateAABB();
		void scaleAABB(glm::vec3 scale);

		void setAABBextentX(float x);
		void setAABBextentY(float y);
		void setAABBextentZ(float z);

		void setAABBextents(glm::vec3 newExtents);

		void setAABBoffsetX(float x);
		void setAABBoffsetY(float y);
		void setAABBoffsetZ(float z);

		void rotateAABB(Degrees deg, Axis axis);

		void setAABBoffsets(glm::vec3 newOffset);

		void setName(std::string name);
		const std::string& getName() const;

		unsigned int getID() const;
		static unsigned int getNextID();

	};
}


#endif // !GOBJECT_CLASS_H