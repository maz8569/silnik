#include "Rendering/Animation.h"

namespace GameEngine {

	Animation::Animation(const std::string& animationPath, Ref<Model> model)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
		assert(scene && scene->mRootNode);
		auto animation = scene->mAnimations[0];
		m_Duration = animation->mDuration;
		m_TicksPerSecond = animation->mTicksPerSecond;
		aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
		globalTransformation = globalTransformation.Inverse();
		ReadHeirarchyData(m_RootNode, scene->mRootNode);
		ReadMissingBones(animation, model);
	}

}