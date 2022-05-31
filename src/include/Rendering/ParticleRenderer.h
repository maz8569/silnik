#pragma once

#include <Scene/Particle.h>
#include <vector>
#include <Shaders/Shader.h>

namespace GameEngine {

	class ParticleRenderer
	{
	private:

		std::vector<Ref<ParticleSystem>> m_particleSystems;
		Ref<Shader> m_particleShader;

	public:

		void Render();

	};

}