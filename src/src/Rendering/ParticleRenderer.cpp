#include "Rendering/ParticleRenderer.h"

namespace GameEngine {

	ParticleRenderer::ParticleRenderer()
	{
		m_particleShader = CreateRef<Shader>("res/shaders/particle.vert", "res/shaders/particle.frag");
		m_particleShader->setInt("ourTexture", 0);
	}

	void ParticleRenderer::addParticleSystem(Ref<ParticleSystem> syst)
	{
		m_particleSystems.push_back(syst);
	}

	void ParticleRenderer::Render(Ref<Camera> camera)
	{
		m_particleShader->use();

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glActiveTexture(GL_TEXTURE0);

		m_particleShader->setVec3("CameraRight_worldspace", camera->Right);
		m_particleShader->setVec3("CameraUp_worldspace", camera->Up);
		m_particleShader->setVec3("cameraPos", camera->Position);

		m_particleShader->setMat4("view", camera->m_viewMatrix);
		m_particleShader->setMat4("projection", camera->m_projectionMatrix);

		for (Ref<ParticleSystem> syst : m_particleSystems)
		{
			syst->activateTexture();

			syst->bindVAO();

			glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, syst->getAmount());

		}

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}