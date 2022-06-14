#pragma once

#include <vector>
#include "Rendering/Model.h"
#include "Scene/SceneGraph.h"
#include <Physics/Collisions/AABB.h>
#include <typedefs.h>
#include <string>

namespace GameEngine {
	class Collision;
	class Scene;

	class GObject
	{
	private:
		std::vector<Ref<GObject>> m_children;
		Ref<GObject> m_parent;
		Transform m_transform;
		bool m_dirty;

	public:

		void add_child(Ref<GObject> child);
		void add_parent(Ref<GObject> parent);
		Ref<GObject> get_parent();
		void update_transform(const Transform& parent_transform, bool dirty);
		Transform& get_transform();
		void set_local_position(const glm::vec3& newPosition);
		void increment_local_position(const glm::vec3& newPosition);
		void set_local_rotation(const glm::vec3& newRotation);
		void set_local_rotationX(const float& x);
		void set_local_rotationY(const float& y);
		void set_local_rotationZ(const float& z);
		void set_local_scale(glm::vec3 newScale);
		std::vector<Ref<GObject>> get_children();

		std::vector<Ref<GComponent>> components;
		void addComponent(Ref<GComponent> comp);

	private:

		std::string m_name;
		std::string m_tag;
		unsigned int ID;
		glm::vec3 m_color;

		Ref<Model> m_model;
		Ref<AABB> m_aabb;

		unsigned int VBO, VAO;
		glm::vec3 offset;

		void rotateAABBY(Degrees deg);
		void rotateAABBZ(Degrees deg);
		void rotateAABBX(Degrees deg);

	public:
		void rotateAABB(Degrees deg, Axis axis);

		Scene* scene;

		Ref<Collision> m_colman;
		Ref<Shader> shader = nullptr;

		bool cast_shadow = true;

		void addToScene(Ref<Collision> coll);

		GObject();
		explicit GObject(std::string modelName);
		~GObject();

		void set_tag(std::string newTag);
		void set_color(glm::vec3 c);

		glm::vec3 get_color();

		void setModel(Ref<Model> model);
		Ref<Model> getModel();
		Ref<AABB> getAABB() const;

		virtual void render(Ref<Shader> shader);
		virtual void Update(float dt);

		virtual void OnCollisionEnter(GObject* other);
		virtual void OnCollisionStay(GObject* other);
		virtual void OnCollisionExit(GObject* other);

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

		void setAABBrotation(Degrees deg, Axis axis);

		void setAABBoffsets(glm::vec3 newOffset);

		void setAABB(Ref<AABB> aabb);

		template<typename T>
		inline Ref<T> GetComponent() const
		{
			for (auto component : components)
			{
				if (Ref<T> comp = Cast<T>(component))
					return comp;
			}

			return nullptr;
		}

		void setName(std::string name);
		const std::string& getName() const;

		unsigned int getID() const;
		static unsigned int getNextID();

	};
}