#include "Scene/Particle.h"

namespace GameEngine {

	static float g_vertex_buffer_data[] =
	{ 0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f };

	Particle::Particle()
		: pos(0.0f), speed(0.0f), color(0.0f), size(0.0f), life(0.0f)
	{

	}

	Particle::Particle(glm::vec3 position, glm::vec3 velocity, glm::vec4 Color, float s, float l)
		: pos(position), speed(velocity), color(Color), size(s), life(l)
	{
	}

	bool Particle::Update(float dt)
	{

		glm::vec3 change = speed;
		change *= dt;

		pos += change;

		elapsedTime += dt;

		return elapsedTime < life;
	}

	ParticleSystem::ParticleSystem(unsigned int _amount, Ref<GTexture> particleTexture)
		: GComponent(), amount(_amount), m_particleTexture(particleTexture), m_vertVbo(GL_ARRAY_BUFFER), m_elseVbo(GL_ARRAY_BUFFER)
	{
		m_vao.Bind();

		m_vertVbo.Bind();
		m_vertVbo.BufferData(sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
		m_vertVbo.VertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		m_vao.Unbind();

		for (unsigned int i = 0; i < amount; i++)
		{
			particles.push_back(Particle());
		}

	}

	void ParticleSystem::setParent(GObject* newParent)
	{
		GComponent::setParent(newParent);
	}

	void ParticleSystem::Update(float dt)
	{
		
		for (Particle& particle : particles)
		{
			particle.Update(dt);
		}
	}

}