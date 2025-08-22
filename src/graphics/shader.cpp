
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include "shader.h"
#include <format>

namespace Graphics
{
	std::string Shader::readFile(const std::string &file_path)
	{
		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open(file_path);

		std::stringstream file_stream;
		file_stream << file.rdbuf();

		file.close();
		return file_stream.str();
	}

	void Shader::checkCompileErrors(GLuint shader, const std::string &type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "[Error] SHADER::COMPILATION_ERROR -> " << type << "\n - "
						  << infoLog << "\n";
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "[Error] - SHADER::PROGRAM_LINKING_ERROR -> " << type << "\n - "
						  << infoLog << "\n";
			}
		}
	}

	Shader::Shader(const std::string &vertex_path, const std::string &fragment_path)
	{
		std::string vertex_code;
		std::string fragment_code;
		try
		{
			vertex_code = readFile(vertex_path);
			fragment_code = readFile(fragment_path);
		}
		catch (std::ifstream::failure &e)
		{
			std::cout << "[Error] - SHADER::FILE_NOT_SUCCESSFULLY_READ -> " << e.what() << "\n";
		}

		const char *vShaderCode = vertex_code.c_str();
		const char *fShaderCode = fragment_code.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// shader Program
		id = glCreateProgram();
		glAttachShader(id, vertex);
		glAttachShader(id, fragment);
		glLinkProgram(id);
		checkCompileErrors(id, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Shader::setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
	}

	void Shader::setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}

	void Shader::setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(id, name.c_str()), value);
	}

	void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
	}

	void Shader::setVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
	}

	void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
	}

	void Shader::setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
	}

	void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
	}

	void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
	}

	void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::use() const
	{
		glUseProgram(id);
	}

}