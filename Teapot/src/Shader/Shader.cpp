#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>

namespace Teapot
{
    Shader::Shader(const std::string& filepath)
        : m_filepath(filepath)
    {
        ShaderProgramSource source = ParseShader(filepath);
        m_rendererID = CreateShader(source.VertexSource, source.FragmentSource);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_rendererID);
    }

    ShaderProgramSource Shader::ParseShader(const std::string& filepath) const
    {
        enum class ShaderType
        {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

        std::ifstream stream(filepath);
        std::string line;
        std::array<std::stringstream, 2> ss{};
        ShaderType type = ShaderType::NONE;

        while (getline(stream, line))
        {
            if (line.contains("#shader"))
            {
                if (line.contains("vertex"))
                    type = ShaderType::VERTEX;
                else if (line.contains("fragment"))
                    type = ShaderType::FRAGMENT;
            }
            else
            {
                ss[static_cast<int>(type)] << line << '\n';
            }
        }

        return { ss[0].str(), ss[1].str() };
    }

    void Shader::Bind() const
    {
        glUseProgram(m_rendererID);
    }

    void Shader::Unbind() const
    {
        glUseProgram(0);
    }

    void Shader::SetUniform1i(const std::string& name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void Shader::SetUniform1f(const std::string& name, float value)
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
    {
        glUniform3f(GetUniformLocation(name), v0, v1, v2);
    }

    void Shader::SetUniformVec3f(const std::string& name, const glm::vec3& vector)
    {
        glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
    }

    void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
    {
        glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
    }

    void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    int  Shader::GetUniformLocation(const std::string& name)
    {
        if (m_uniformLocationCache.contains(name))
            return m_uniformLocationCache[name];
        int location = glGetUniformLocation(m_rendererID, name.c_str());
        if (location == -1)
            std::cout << "No active uniform variable with name " << name << " found" << std::endl;

        m_uniformLocationCache[name] = location;
        return location;
    }

    unsigned int Shader::CompileShader(unsigned int type, const std::string& source) const
    {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        // Error handling
        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        std::cout << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader compile status: " << result << std::endl;
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            auto message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cout
                << "Failed to compile "
                << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                << "shader"
                << std::endl;
            std::cout << message << std::endl;
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) const
    {
        // create a shader program
        unsigned int program = glCreateProgram();
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);

        GLint program_linked;

        glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
        std::cout << "Program link status: " << program_linked << std::endl;
        if (program_linked != GL_TRUE)
        {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetProgramInfoLog(program, 1024, &log_length, message);
            std::cout << "Failed to link program" << std::endl;
            std::cout << message << std::endl;
        }

        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }
}