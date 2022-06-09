#pragma once

#include "typedefs.h"
#include "WindowManager.h"
#include <Shaders/Shader.h>
#include <Texture/BasicTexture.h>

namespace GameEngine {
	
	class Camera;
	
	class MasterRenderer
	{
	public: 
		void DrawQuad(const glm::vec3& pos);
		void finishRender();
		
		void setQuadShader(Ref<Shader> shader);

	private:

	};
}

