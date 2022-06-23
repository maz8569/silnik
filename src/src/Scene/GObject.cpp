#include "Scene/GObject.h"
#include "Physics/Collisions/Collision.h"
#include <Utils/ResourceManager.h>
#include <Scene/Scene.h>

namespace GameEngine {

	void GameEngine::GObject::add_child(Ref<GObject> child)
	{
		m_children.push_back(child);
	}

	void GameEngine::GObject::add_parent(Ref<GObject> parent)
	{
		m_parent = parent;
	}

	Ref<GObject> GameEngine::GObject::get_parent()
	{
		return m_parent;
	}

	void GameEngine::GObject::update_transform(const Transform& parent_transform, bool dirty)
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

	Transform& GameEngine::GObject::get_transform()
	{
		return m_transform;
	}

	void GameEngine::GObject::set_local_position(const glm::vec3& newPosition)
	{
		m_transform.m_position = newPosition;
		m_dirty = true;
	}

	void GameEngine::GObject::increment_local_position(const glm::vec3& newPosition)
	{
		m_transform.m_position += newPosition;
		m_dirty = true;
	}

	void GameEngine::GObject::set_local_rotation(const glm::vec3& newRotation)
	{
		m_transform.m_rotation = newRotation;
		m_dirty = true;
	}

	void GameEngine::GObject::set_local_rotationX(const float& x)
	{
		m_transform.m_rotation.x = x;
		m_dirty = true;
	}

	void GameEngine::GObject::set_local_rotationY(const float& y)
	{
		m_transform.m_rotation.y = y;
		m_dirty = true;
	}

	void GameEngine::GObject::set_local_rotationZ(const float& z)
	{
		m_transform.m_rotation.z = z;
		m_dirty = true;
	}

	void GameEngine::GObject::set_local_scale(glm::vec3 newScale)
	{
		m_transform.m_scale = newScale;
		m_dirty = true;

		scaleAABB(newScale);
	}

	std::vector<Ref<GObject>> GameEngine::GObject::get_children()
	{
		return m_children;
	}

	void GameEngine::GObject::addComponent(Ref<GComponent> comp)
	{
		components.push_back(comp);
		comp->setParent(this);
	}

	void GameEngine::GObject::rotateAABBY(Degrees deg)
	{
		if (deg != Degrees::D180)
		{
			m_aabb->rotateAABBY();
		}
		float x = offset.x;

		switch (deg)
		{
		case GameEngine::Degrees::D90:
			offset.x = offset.z;
			offset.z = -x;
			break;
		case GameEngine::Degrees::D180:
			offset.x = -x;
			offset.z = -offset.z;
			break;
		case GameEngine::Degrees::D270:
			offset.x = offset.z;
			offset.z = x;
			break;
		default:
			break;
		}


	}

	void GameEngine::GObject::rotateAABBZ(Degrees deg)
	{
		if (deg != Degrees::D180)
		{
			m_aabb->rotateAABBZ();
		}
		float x = offset.x;

		switch (deg)
		{
		case GameEngine::Degrees::D90:
			offset.x = offset.y;
			offset.y = -x;
			break;
		case GameEngine::Degrees::D180:
			offset.x = -x;
			offset.y = -offset.y;
			break;
		case GameEngine::Degrees::D270:
			offset.x = offset.y;
			offset.y = x;
			break;
		default:
			break;
		}
	}

	void GameEngine::GObject::rotateAABBX(Degrees deg)
	{
		if (deg != Degrees::D180)
		{
			m_aabb->rotateAABBX();
		}
		float y = offset.y;

		switch (deg)
		{
		case GameEngine::Degrees::D90:
			offset.y = offset.z;
			offset.z = -y;
			break;
		case GameEngine::Degrees::D180:
			offset.y = -y;
			offset.z = -offset.z;
			break;
		case GameEngine::Degrees::D270:
			offset.y = offset.z;
			offset.z = y;
			break;
		default:
			break;
		}
	}

	void GameEngine::GObject::addToScene(Ref<Collision> coll)
	{
		m_colman = coll;
		if (m_aabb != nullptr)
			m_colman->AddAABB(m_aabb);
	}

	GameEngine::GObject::GObject() : m_dirty(true), m_transform(Transform()), m_color({ 1, 1, 1 })
	{
	}

	GameEngine::GObject::GObject(std::string modelName) : m_dirty(true), m_transform(Transform()), m_color({ 1, 1, 1 })
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		if (modelName != "")
		{
			m_model = ResourceManager::getModel(modelName);

			
			if (m_model->GetBoneCount() > 0)
			{
				std::cout << "\nanimator" << m_model->GetBoneCount() <<" bones\n";
				Ref<Animation> danceAnimation = CreateRef<Animation>(m_model->path, m_model);
				Ref<Animator> animator = CreateRef<Animator>(danceAnimation);
				addComponent(animator);
			}
			

			m_aabb = CreateRef<AABB>(generateAABB(m_model));
			m_aabb->parent = this;
			//std::cout << m_aabb->center.x << " " << m_aabb->center.y << " " << m_aabb->center.z << " " << std::endl;
			offset = m_aabb->center;
			//std::cout << offset.x << " " << offset.y << " " << offset.z << " " << std::endl;
			//std::cout << m_aabb->extents.x << " " << m_aabb->extents.y << " " << m_aabb->extents.z << " " << std::endl;

			MoveColliders();

			recalculateAABB();

		}

	}

	GameEngine::GObject::~GObject()
	{
		if (m_model != nullptr)
		{
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
		}

	}

	void GameEngine::GObject::set_tag(std::string newTag)
	{
		m_aabb->tag = newTag;
	}

	void GameEngine::GObject::set_color(glm::vec3 c)
	{
		m_color = c;
	}

	glm::vec3 GameEngine::GObject::get_color()
	{
		return m_color;
	}

	void GameEngine::GObject::setModel(Ref<Model> model)
	{
		m_model = model;
	}

	Ref<Model> GameEngine::GObject::getModel()
	{
		return m_model;
	}

	Ref<AABB> GameEngine::GObject::getAABB() const
	{
		return m_aabb;
	}

	void GameEngine::GObject::render(Ref<Shader> shader)
	{
		shader->use();
		shader->setMat4("model", get_transform().m_world_matrix);
		if (m_model != nullptr)
		{
			shader->setVec3("color", m_color);
			Ref<Animator> anim = GetComponent<Animator>();
			if (anim != nullptr)
			{
				auto transforms = anim->GetFinalBoneMatrices();
				for (int i = 0; i < transforms.size(); ++i)
					shader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
			}
			m_model->Draw(shader);
		}

		if (m_aabb != nullptr)
		{
			if (m_aabb->render_AABB)
			{
				shader->setMat4("model", glm::translate(glm::mat4(1.0f), get_transform().m_position));
				shader->setVec3("color", { 1, 1, 1 });
				glBindVertexArray(VAO);
				glDrawArrays(GL_LINE_STRIP, 0, 20);
			}
		}
	}

	void GameEngine::GObject::Update(float dt)
	{

		for (auto comp : components)
		{
			comp->Update(dt);
		}

		MoveColliders();
	}

	void GameEngine::GObject::OnCollisionEnter(GObject* other)
	{
		for (auto comp : components)
		{
			comp->OnCollisionEnter(other);
		}
	}

	void GameEngine::GObject::OnCollisionStay(GObject* other)
	{
		for (auto comp : components)
		{
			comp->OnCollisionStay(other);
		}
	}

	void GameEngine::GObject::OnCollisionExit(GObject* other)
	{
		for (auto comp : components)
		{
			comp->OnCollisionExit(other);
		}
	}

	void GameEngine::GObject::MoveColliders()
	{
		if (m_aabb != nullptr)
			m_aabb->center = get_transform().m_position + offset;
	}

	void GameEngine::GObject::set_render_AABB(bool set)
	{
		if (m_aabb != nullptr)
			m_aabb->render_AABB = set;
	}

	void GameEngine::GObject::recalculateAABB()
	{
		m_aabb->center = offset;

		float vertices[] = {
			m_aabb->center.x + m_aabb->extents.x, m_aabb->center.y + m_aabb->extents.y, m_aabb->center.z + m_aabb->extents.z,
			m_aabb->center.x - m_aabb->extents.x, m_aabb->center.y + m_aabb->extents.y, m_aabb->center.z + m_aabb->extents.z,
			m_aabb->center.x - m_aabb->extents.x, m_aabb->center.y - m_aabb->extents.y, m_aabb->center.z + m_aabb->extents.z,
			m_aabb->center.x + m_aabb->extents.x, m_aabb->center.y - m_aabb->extents.y, m_aabb->center.z + m_aabb->extents.z,
			m_aabb->center.x + m_aabb->extents.x, m_aabb->center.y + m_aabb->extents.y, m_aabb->center.z + m_aabb->extents.z,

			m_aabb->center.x + m_aabb->extents.x, m_aabb->center.y + m_aabb->extents.y, m_aabb->center.z - m_aabb->extents.z,
			m_aabb->center.x - m_aabb->extents.x, m_aabb->center.y + m_aabb->extents.y, m_aabb->center.z - m_aabb->extents.z,
			m_aabb->center.x - m_aabb->extents.x, m_aabb->center.y - m_aabb->extents.y, m_aabb->center.z - m_aabb->extents.z,
			m_aabb->center.x + m_aabb->extents.x, m_aabb->center.y - m_aabb->extents.y, m_aabb->center.z - m_aabb->extents.z,
			m_aabb->center.x + m_aabb->extents.x, m_aabb->center.y + m_aabb->extents.y, m_aabb->center.z - m_aabb->extents.z,

			m_aabb->center.x + m_aabb->extents.x, m_aabb->center.y + m_aabb->extents.y, m_aabb->center.z + m_aabb->extents.z,
			m_aabb->center.x - m_aabb->extents.x, m_aabb->center.y + m_aabb->extents.y, m_aabb->center.z + m_aabb->extents.z,
			m_aabb->center.x - m_aabb->extents.x, m_aabb->center.y + m_aabb->extents.y, m_aabb->center.z - m_aabb->extents.z,
			m_aabb->center.x + m_aabb->extents.x, m_aabb->center.y + m_aabb->extents.y, m_aabb->center.z - m_aabb->extents.z,
			m_aabb->center.x + m_aabb->extents.x, m_aabb->center.y + m_aabb->extents.y, m_aabb->center.z + m_aabb->extents.z,

			m_aabb->center.x + m_aabb->extents.x, m_aabb->center.y - m_aabb->extents.y, m_aabb->center.z + m_aabb->extents.z,
			m_aabb->center.x - m_aabb->extents.x, m_aabb->center.y - m_aabb->extents.y, m_aabb->center.z + m_aabb->extents.z,
			m_aabb->center.x - m_aabb->extents.x, m_aabb->center.y - m_aabb->extents.y, m_aabb->center.z - m_aabb->extents.z,
			m_aabb->center.x + m_aabb->extents.x, m_aabb->center.y - m_aabb->extents.y, m_aabb->center.z - m_aabb->extents.z,
			m_aabb->center.x + m_aabb->extents.x, m_aabb->center.y - m_aabb->extents.y, m_aabb->center.z + m_aabb->extents.z
		};

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		MoveColliders();
	}

	void GameEngine::GObject::scaleAABB(glm::vec3 scale)
	{
		m_aabb->scaleAABB(scale);

		recalculateAABB();
	}

	void GameEngine::GObject::setAABBextentX(float x)
	{
		m_aabb->setXExtent(x);
		recalculateAABB();
	}

	void GameEngine::GObject::setAABBextentY(float y)
	{
		m_aabb->setYExtent(y);
		recalculateAABB();
	}

	void GameEngine::GObject::setAABBextentZ(float z)
	{
		m_aabb->setZExtent(z);
		recalculateAABB();
	}

	void GameEngine::GObject::setAABBextents(glm::vec3 newExtents)
	{
		m_aabb->setExtents(newExtents);
		recalculateAABB();
	}

	void GameEngine::GObject::setAABBoffsetX(float x)
	{
		offset.x = x;
	}

	void GameEngine::GObject::setAABBoffsetY(float y)
	{
		offset.y = y;
	}

	void GameEngine::GObject::setAABBoffsetZ(float z)
	{
		offset.z = z;
	}

	void GameEngine::GObject::setAABBrotation(Degrees deg, Axis axis)
	{
		switch (axis)
		{
		case GameEngine::Axis::X:
			if (m_aabb->Xrot != deg)
			{
				m_aabb->Xrot = deg;
				rotateAABB(deg, axis);
			}
			break;
		case GameEngine::Axis::Y:
			if (m_aabb->Yrot != deg)
			{
				m_aabb->Yrot = deg;
				rotateAABB(deg, axis);
			}
			break;
		case GameEngine::Axis::Z:
			if (m_aabb->Zrot != deg)
			{
				m_aabb->Zrot = deg;
				rotateAABB(deg, axis);
			}
			break;
		default:
			break;
		}
	}

	void GameEngine::GObject::rotateAABB(Degrees deg, Axis axis)
	{
		switch (axis)
		{
		case GameEngine::Axis::X:
			rotateAABBX(deg);
			break;
		case GameEngine::Axis::Y:
			rotateAABBY(deg);
			break;
		case GameEngine::Axis::Z:
			rotateAABBZ(deg);
			break;
		default:
			break;
		}

		MoveColliders();

		//std::cout <<"\n\n" << m_aabb->center.x << " " << m_aabb->center.y << " " << m_aabb->center.z << " " << std::endl;
		//std::cout << offset.x << " " << offset.y << " " << offset.z << " " << std::endl;


		recalculateAABB();
	}

	void GameEngine::GObject::setAABBoffsets(glm::vec3 newOffset)
	{
		offset = newOffset;
	}

	void GameEngine::GObject::setAABB(Ref<AABB> aabb)
	{
		m_aabb = aabb;
		m_aabb->parent = this;
		offset = m_aabb->center;

		MoveColliders();

		recalculateAABB();
	}
}