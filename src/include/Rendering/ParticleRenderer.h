#pragma once

#include <Scene/Particle.h>
#include <vector>
#include <Utils/ResourceManager.h>
#include <Scene/Camera.h>

namespace GameEngine {

	class ParticleRenderer
	{
	private:

		std::vector<Ref<ParticleSystem>> m_particleSystems;
		Ref<Shader> m_particleShader;

	public:

		ParticleRenderer();
		~ParticleRenderer();

		void addParticleSystem(Ref<ParticleSystem> syst);

		void Render(Ref<Camera> camera);

	};

}