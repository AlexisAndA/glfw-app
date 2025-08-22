#pragma once

#include <vector>

#include "light.h"
#include "object.h"

class World
{
private:
public:
	std::vector<Object *> objects;
	std::vector<Light *> lights;
	World() {}

	void AddLight(Light *light)
	{
		lights.push_back(light);
	}

	void AddObject(Object *object)
	{
		objects.push_back(object);
	}

	void RenderDepthMaps(Window &window)
	{
		for (Light *light : lights)
		{
			glm::mat4 lightProjection, lightView;
			glm::mat4 lightSpaceMatrix;
			lightProjection = glm::ortho(-10.0f * 5, 10.0f * 5, -10.0f * 5, 10.0f * 5, NEAR_PLANE, FAR_PLANE);
			lightView = glm::lookAt(light->Position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
			lightSpaceMatrix = lightProjection * lightView;

			light->depth_shader.use();
			light->depth_shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, light->framebuffer);
			glClear(GL_DEPTH_BUFFER_BIT);
			// glActiveTexture(GL_TEXTURE0);
			// glBindTexture(GL_TEXTURE_2D, woodTexture);
			RenderWithDepthShader(*light);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void RenderWithDepthShader(Light light)
	{
		for (Object *object : objects)
		{
			object->RenderDepth(light);
		}
	}

	void Render(Window &window, Camera &camera)
	{
		glViewport(0, 0, window.width, window.height);

		for (Object *object : objects)
		{
			object->Render(window, camera, lights);
		}
		for (Light *light : lights)
		{

			light->Render(window, camera);
		}
	}

	void CleanUp()
	{
		for (Object *object : objects)
		{
			if (object->HasChildren())
			{
				object->DeleteChildren();
			}
			delete object;
		}

		for (Light *light : lights)
		{
			delete light;
		}
	}
};