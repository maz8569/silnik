#include "Scene/GObject.h"
#include "Physics/Collisions/Collision.h"

using namespace GameEngine;

GameEngine::GObject::GObject(): SceneNode(), m_color({1, 1, 1})
{
}

GameEngine::GObject::GObject(std::shared_ptr<Model> model, std::shared_ptr<Collision> colMan): m_model(model),  SceneNode(), m_color({ 1, 1, 1 })
{
	m_aabb = std::make_shared<AABB>(generateAABB(model));
	m_colman = colMan;
	m_colman->AddAABB(m_aabb);
	m_aabb->parent = this;
	std::cout << m_aabb->center.x << " " << m_aabb->center.y << " " << m_aabb->center.z << " " << std::endl;
	offset = m_aabb->center;
	std::cout << m_aabb->extents.x << " " << m_aabb->extents.y << " " << m_aabb->extents.z << " " << std::endl;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	recalculateAABB();

	MoveColliders();
}

GameEngine::GObject::~GObject()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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

void GameEngine::GObject::setModel(std::shared_ptr<Model> model)
{
	m_model = model;
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
		m_model->Draw(shader);
		if (m_aabb != nullptr && render_AABB)
		{
			shader->setVec3("color", { 1, 1, 1 });
			glBindVertexArray(VAO);
			glDrawArrays(GL_LINE_STRIP, 0, 20);
		}
	}

	auto children = get_children();

	for (uint32_t i = 0; i < children.size(); ++i)
	{
		children[i]->render();
	}
}

void GameEngine::GObject::Update()
{
	MoveColliders();
}

void GameEngine::GObject::reactOnCollision(GObject* other)
{
	//std::cout << "collide";
	//other->set_color({ 1, 0, 0 });
}

void GameEngine::GObject::MoveColliders()
{
	m_aabb->center = get_transform().m_position + offset;
}

void GameEngine::GObject::set_render_AABB(bool set)
{
	render_AABB = set;
}

void GameEngine::GObject::recalculateAABB()
{

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

void GameEngine::GObject::setAABBoffsets(glm::vec3 newOffset)
{
	offset = newOffset;
}
