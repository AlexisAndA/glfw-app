#include "color.h"

namespace Graphics
{
	glm::vec3 Color::ToVec3() const
	{
		return glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);
	}

	glm::vec4 Color::ToVec4() const
	{
		return glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}
}