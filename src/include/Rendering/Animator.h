#pragma once

#include <Scene/GComponent.h>
#include "Animation.h"

namespace GameEngine {

	class Animator : public GComponent
	{
	public:
		Animator(Ref<Animation> animation);

		virtual void Update(float dt);

		void UpdateAnimation(float dt);

		void PlayAnimation(Ref<Animation> pAnimation);

		void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

		std::vector<glm::mat4> GetFinalBoneMatrices();

	private:
		std::vector<glm::mat4> m_FinalBoneMatrices;
		Ref<Animation> m_CurrentAnimation;
		float m_CurrentTime;
		float m_DeltaTime;

	};

}
