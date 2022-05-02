#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>
#include <stb_image.h>
#include <iostream>

namespace GameEngine {

	class CubemapTexture
	{
	public:

		void CreateCubemapTexture( const std::vector<std::string>& faces);

		inline GLuint GetID() const {
			return m_TextureID;
		}

	private:

		GLuint m_TextureID;

		std::vector<std::string> m_CubeFacePaths;

	};

}