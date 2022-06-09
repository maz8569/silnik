#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Scene/GComponent.h>
#include <Texture/Texture.h>
#include <Utils/VertexArray.h>
#include <Utils/VertexBuffer.h>

namespace GameEngine {

	struct Particle
	{
		glm::vec3 pos, speed;
		glm::vec4 color;
		float size, life;
		float elapsedTime = 0;

		Particle();
		Particle(glm::vec3 position, glm::vec3 velocity, glm::vec4 Color, float s, float l);

		bool Update(float dt);

	};


	class ParticleSystem : public GComponent
	{

	private:

		Ref<GTexture> m_particleTexture;

		std::vector<Particle> particles;
		unsigned int amount;

		VertexArray m_vao;
		VertexBuffer m_vertVbo;
		VertexBuffer m_elseVbo;

		std::vector<float> positioncolors;

	public:

		ParticleSystem(unsigned int _amount, std::string textureName);
		~ParticleSystem();

		virtual void setParent(GObject* newParent);

		virtual void Update(float dt);

		Ref<GTexture> getTexture();
		void activateTexture();

		void respawnParticle(Particle& particle, glm::vec3 offset);

		void bindVAO();
		void unbindVAO();

		unsigned int getAmount();
		void setAmount(unsigned int am);

	};

}