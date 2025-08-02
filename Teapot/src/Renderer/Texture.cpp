#include"Texture.h"
#include <iostream>
#include <algorithm>

namespace Teapot
{
	Texture::Texture(const char* filepath, const std::string& texType, GLuint unitType)
		: type(texType)
		, path(filepath)
		, unit(unitType)
	{
		std::ranges::replace(path.begin(), path.end(), '\\', '/');
		std::cout << "Path Adress: " << path << " Texture Type: " << type << std::endl;

		glGenTextures(1, &ID);

		int width{};
		int height{};
		int nrComponents{};

		stbi_set_flip_vertically_on_load(true);
		unsigned char* bytes = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
		if (bytes)
		{
			GLenum format{};
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, ID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, bytes);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(bytes);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(bytes);
		}
	}

	void Texture::TexUnit(Shader& shader) const
	{
		shader.SetUniform1i(type, unit);
	}

	void Texture::Bind() const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	void Texture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Delete() const
	{
		glDeleteTextures(1, &ID);
	}

}
