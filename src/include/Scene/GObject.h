#pragma once
#ifndef GOBJECT_CLASS_H
#define GOBJECT_CLASS_H

#include <memory>
#include <vector>
//#include <Collision.h>
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

	class GObject : public SceneNode
	{
	private:
		//std::vector<std::shared_ptr<Collision>> collisions;
		//std::shared_ptr<Plan> boundingVolume;
		Ref<Model> m_model;
		Ref<AABB> m_aabb;
		glm::vec3 m_color;
		//Ref<Shader> m_shader;
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
		explicit GObject(std::shared_ptr<Model> model, std::shared_ptr<Collision> colMan);
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