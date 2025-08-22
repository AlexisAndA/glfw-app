#pragma once

#include <glm/glm.hpp>
#include <cstdint>

namespace Graphics
{

	struct Color
	{

		uint8_t r = 255;
		uint8_t g = 255;
		uint8_t b = 255;
		uint8_t a = 255;

		constexpr Color()
		{
			r = 255;
			g = 255;
			b = 255;
			a = 255;
		}

		constexpr Color(uint8_t value) : r(value), g(value), b(value), a(value) {}

		constexpr Color(uint8_t r,
						uint8_t g,
						uint8_t b,
						uint8_t a) : r(r), g(g), b(b), a(a) {}

		glm::vec3 ToVec3() const;

		glm::vec4 ToVec4() const;
	};
}