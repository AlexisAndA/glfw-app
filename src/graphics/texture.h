#pragma once

#include <string>

namespace Graphics
{

	class Texture
	{
	private:
		unsigned int id;

	public:
		Texture(std::string file_path);

		void BindToUnit(GLenum unit);
	};
}