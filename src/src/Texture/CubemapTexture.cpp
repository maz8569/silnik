#include "Texture/CubemapTexture.h"

void GameEngine::CubemapTexture::CreateCubemapTexture(const std::vector<std::string>& faces)
{
	unsigned char* image_data;
	int width, height, channels;

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

	m_CubeFacePaths = faces;

	for (int i = 0; i < faces.size(); i++)
	{
		stbi_set_flip_vertically_on_load(false);
		image_data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);

		if (image_data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
				height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
			stbi_image_free(image_data);
		}

		else
		{
			std::cout << "\nFailed to load image : " << faces[i];
			stbi_image_free(image_data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}
}
