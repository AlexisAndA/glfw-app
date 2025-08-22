
#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Graphics
{
	/// @brief This class represents a complete OpenGL shader with both vertex and fragment shaders.
	class Shader
	{
	private:
		unsigned int id;

		std::string readFile(const std::string &file_path);

		/// @brief utility function for checking shader compilation/linking errors.
		void checkCompileErrors(GLuint shader, const std::string &type);

	public:
		Shader(const std::string &vertex_path, const std::string &fragment_path);

		void use() const;

		void setBool(const std::string &name, bool value) const;

		void setInt(const std::string &name, int value) const;

		void setFloat(const std::string &name, float value) const;

		void setVec2(const std::string &name, const glm::vec2 &value) const;

		void setVec2(const std::string &name, float x, float y) const;

		void setVec3(const std::string &name, const glm::vec3 &value) const;

		void setVec3(const std::string &name, float x, float y, float z) const;

		void setVec4(const std::string &name, const glm::vec4 &value) const;

		void setVec4(const std::string &name, float x, float y, float z, float w) const;

		void setMat2(const std::string &name, const glm::mat2 &mat) const;

		void setMat3(const std::string &name, const glm::mat3 &mat) const;

		void setMat4(const std::string &name, const glm::mat4 &mat) const;

		inline unsigned int GetId() const
		{
			return id;
		}
	};
}