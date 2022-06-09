#include "Scene/Particle.h"
#include <Scene/GObject.h>
#include <Utils/ResourceManager.h>

namespace GameEngine {

	static float g_vertex_buffer_data[] =
	{	 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f, };

	Particle::Particle()
		: pos(0.0f), speed(0.0f), color(0.0f), size(1.0f), life(0.0f)
	{

	}

	Particle::Particle(glm::vec3 position, glm::vec3 velocity, glm::vec4 Color, float s, float l)
		: pos(position), speed(velocity), color(Color), size(s), life(l)
	{
	}

	bool Particle::Update(float dt)
	{

		glm::vec3 change = speed;

		pos += change;

		life -= dt;

		return life > 0;
	}

	ParticleSystem::ParticleSystem(unsigned int _amount, std::string textureName)
		: GComponent(), amount(_amount), m_vertVbo(GL_ARRAY_BUFFER), m_elseVbo(GL_ARRAY_BUFFER)
	{
		m_particleTexture = ResourceManager::getTexture(textureName);
		
		m_vao.Bind();

		m_vertVbo.Bind();
		m_vertVbo.BufferData(sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
		m_vertVbo.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		m_elseVbo.Bind();
		m_elseVbo.BufferData(amount * 8 * sizeof(float), NULL, GL_STREAM_DRAW);

		m_vao.Unbind();

		for (unsigned int i = 0; i < amount; i++)
		{
			particles.push_back(Particle());
		}

	}

	ParticleSystem::~ParticleSystem()
	{
		positioncolors.clear();

		for (int i = 0; i < amount; i++)
		{
			Particle& particle = particles[i];

			positioncolors.push_back(particle.pos.x);
			positioncolors.push_back(particle.pos.y);
			positioncolors.push_back(particle.pos.z);

			positioncolors.push_back(0);

			positioncolors.push_back(particle.color.r);
			positioncolors.push_back(particle.color.g);
			positioncolors.push_back(particle.color.b);
			positioncolors.push_back(particle.color.a);
		}
		m_vao.Bind();

		m_elseVbo.Bind();
		m_elseVbo.BufferData(amount * 8 * sizeof(float), NULL, GL_STREAM_DRAW);
		m_elseVbo.BufferSubData(0, amount * 8 * sizeof(float), &positioncolors[0]);
		m_elseVbo.VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		m_elseVbo.VertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)4);

		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
		glVertexAttribDivisor(2, 1);

		m_vao.Unbind();

	}

	void ParticleSystem::setParent(GObject* newParent)
	{
		GComponent::setParent(newParent);


		for (unsigned int i = 0; i < amount; i++)
		{
			respawnParticle(particles[i], { 1, 1, 1 });
		}

	}

	void ParticleSystem::Update(float dt)
	{
		positioncolors.clear();

		for ( int i = 0; i < amount; i++)
		{
			Particle& particle = particles[i];
			particle.Update(dt);

			if(particle.life < 0)
				respawnParticle(particle, { 0.1, -1, -0.1 });

			positioncolors.push_back(particle.pos.x);
			positioncolors.push_back(particle.pos.y);
			positioncolors.push_back(particle.pos.z);
			
			positioncolors.push_back(particle.size);
			
			positioncolors.push_back(particle.color.r);
			positioncolors.push_back(particle.color.g);
			positioncolors.push_back(particle.color.b);
			positioncolors.push_back(particle.color.a);
		}
		m_vao.Bind();

		m_elseVbo.Bind();
		m_elseVbo.BufferData(amount * 8 * sizeof(float), NULL, GL_STREAM_DRAW);
		m_elseVbo.BufferSubData(0, amount * 8 * sizeof(float), &positioncolors[0]);
		m_elseVbo.VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		m_elseVbo.VertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)4);

		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
		glVertexAttribDivisor(2, 1);

		m_vao.Unbind();

	}

	Ref<GTexture> ParticleSystem::getTexture()
	{
		return m_particleTexture;
	}

	void ParticleSystem::activateTexture()
	{
		m_particleTexture->Bind();
	}

	void ParticleSystem::respawnParticle(Particle& particle, glm::vec3 offset)
	{
		float random = ((rand() % 100) - 50) / 10.0f;
		float rColor = 0.5f + ((rand() % 100) / 100.0f);
		particle.pos = parent->get_transform().m_position + random * offset;
		particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
		particle.life = 0.5f*(5 - abs( random));
		particle.speed = abs(random) * glm::vec3(0.1, 1, 0.1) * 0.1f;
	}

	void ParticleSystem::bindVAO()
	{
		m_vao.Bind();
	}

	void ParticleSystem::unbindVAO()
	{
		m_vao.Unbind();
	}

	unsigned int ParticleSystem::getAmount()
	{
		return amount;
	}

	void ParticleSystem::setAmount(unsigned int am)
	{
		amount = am;
	}

}