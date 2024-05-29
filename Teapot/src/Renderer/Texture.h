#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include <stdexcept>

#include "Shader.h"

namespace Teapot
{
	class Texture
	{
	public:
		GLuint ID;
		std::string type;
		GLuint unit;
		std::string path;

		Texture(const char* filepath, std::string& texType, GLuint unitType);
		~Texture();

		void TexUnit(Shader& shader, const char* uniform, GLuint _unit) const;
		void Bind() const;
		void Unbind() const;
		void Delete() const;

	private:


	};
}