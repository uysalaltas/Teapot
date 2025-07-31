#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include <stdexcept>

#include "Shader/Shader.h"
#include <iostream>

namespace Teapot
{
	class Texture
	{
	public:
		GLuint ID;
		std::string type{};
		std::string path{};
		GLuint unit;

		Texture(const char* filepath, const std::string& texType, GLuint unitType);
		~Texture() { glDeleteTextures(1, &ID); std::cout << "Deleted Texture \n"; };

		void TexUnit(Shader& shader) const;
		void Bind() const;
		void Unbind() const;
		void Delete() const;
	};
}