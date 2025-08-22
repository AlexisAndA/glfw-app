#pragma once

#include <glm/glm.hpp>

#include "graphics/shader.h"
#include "graphics/color.h"
#include "model.h"
#include "graphics/camera.h"

constexpr unsigned int SHADOW_WIDTH = 1024 * 1.5;
constexpr unsigned int SHADOW_HEIGHT = 1024 * 1.5;

constexpr float NEAR_PLANE = 2.0f;
constexpr float FAR_PLANE = 15.5f;

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,
			1.0f,
			0.0f,
			0.0f,
			1.0f,
			-1.0f,
			-1.0f,
			0.0f,
			0.0f,
			0.0f,
			1.0f,
			1.0f,
			0.0f,
			1.0f,
			1.0f,
			1.0f,
			-1.0f,
			0.0f,
			1.0f,
			0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

/// @brief This class represents a light in 3D space.
class Light
{
private:
	Model model;
	Graphics::Shader shader;

	Graphics::Shader quad_shader;

public:
	unsigned int framebuffer;
	unsigned int depth_map;

	Graphics::Shader depth_shader;

	glm::vec3 InitialPosition;
	glm::vec3 Position;
	Graphics::Color Color;

	Light(glm::vec3 position = glm::vec3(0.0f), Graphics::Color color = Graphics::Color()) : Position(position), InitialPosition(Position), Color(color), model(Model("assets/light.obj")), shader(Graphics::Shader("shaders/light_vertex_shader.glsl", "shaders/light_fragment_shader.glsl")), quad_shader(Graphics::Shader("shaders/quad_shader.vs", "shaders/quad_shader.fs")), depth_shader(Graphics::Shader("shaders/depth_shader.vs", "shaders/depth_shader.fs"))
	{
		quad_shader.use();
		quad_shader.setInt("depthMap", 2);

		glGenFramebuffers(1, &framebuffer);

		glGenTextures(1, &depth_map);
		glBindTexture(GL_TEXTURE_2D, depth_map);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderDebugQuad()
	{
		quad_shader.use();
		quad_shader.setFloat("near_plane", NEAR_PLANE);
		quad_shader.setFloat("far_plane", FAR_PLANE);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depth_map);
		renderQuad();
	}

	void Render(Window &window, Camera &camera)
	{
		if (window.IsKeyPressed(GLFW_KEY_PAGE_UP))
		{
			Position.x += 0.04f;
		}

		if (window.IsKeyPressed(GLFW_KEY_PAGE_DOWN))
		{
			Position.x -= 0.04f;
		}

		if (window.IsKeyPressed(GLFW_KEY_HOME))
		{
			Position.z += 0.04f;
		}

		if (window.IsKeyPressed(GLFW_KEY_END))
		{
			Position.z -= 0.04f;
		}

		shader.use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), (float)window.width / (float)window.height, 0.1f, 100.0f);
		glm::mat4 view = camera.ToView();

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, Position);
		model = glm::scale(model, glm::vec3(0.1f));
		shader.setMat4("model", model);

		shader.setVec4("color", Color.ToVec4());

		this->model.Draw(shader);
	}
};