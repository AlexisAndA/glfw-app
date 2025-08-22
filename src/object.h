#pragma once

#include "model.h"
#include "graphics/window.h"
#include "graphics/camera.h"
#include "light.h"
#include <format>
#include "graphics/texture.h"

class Object
{
private:
	Object *parent = nullptr;
	std::vector<Object *> children;

	Model model;

	Graphics::Texture diffuse_texture;
	Graphics::Texture specular_texture;

	Graphics::Shader shader;

public:
	glm::vec3 Position;
	glm::vec3 GlobalPosition;

	glm::vec3 Rotation = glm::vec3(0.0f);

	Object(std::string model_location, glm::vec3 position = glm::vec3(0.0f), Graphics::Texture diffuse = Graphics::Texture("assets/stone.png"), Graphics::Texture specular = Graphics::Texture("assets/stone.png")) : model(Model(model_location)), Position(glm::vec3(0.0f)), GlobalPosition(position), diffuse_texture(diffuse), specular_texture(specular), shader(Graphics::Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl")) {}

	void SetParent(Object *object)
	{
		parent = object;
	}

	void AddChild(Object *object)
	{
		object->SetParent(this);
		children.push_back(object);
	}

	bool HasChildren()
	{
		return children.empty();
	}

	void DeleteChildren()
	{
		for (Object *child : children)
		{
			delete child;
		}
	}

	void RenderDepth(Light light)
	{
		light.depth_shader.use();

		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, GlobalPosition + Position);

		model = glm::rotate(model, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
		model = glm::rotate(model, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
		model = glm::rotate(model, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis

		model = glm::scale(model, glm::vec3(7.0f));

		light.depth_shader.setMat4("model", model);

		this->model.Draw(light.depth_shader);

		for (Object *object : children)
		{
			object->Rotation = Rotation;
			object->Position = Position;
			object->RenderDepth(light);
		}
	}

	void Render(Window &window, Camera &camera, std::vector<Light *> &lights)
	{
		shader.use();
		shader.setVec3("viewPos", camera.position);
		shader.setFloat("material.shininess", 32.0f);

		shader.setInt("material.diffuse", 0);
		shader.setInt("material.specular", 1);

		/*
		   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
		*/
		// directional light
		shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		int index = 0;

		diffuse_texture.BindToUnit(GL_TEXTURE0);
		specular_texture.BindToUnit(GL_TEXTURE1);
		for (Light *light : lights)
		{
			shader.setVec3(std::format("pointLights[{}].position", index), light->Position);
			shader.setVec3(std::format("pointLights[{}].ambient", index), 0.05f, 0.05f, 0.05f);
			shader.setVec3(std::format("pointLights[{}].diffuse", index), light->Color.ToVec3());
			shader.setVec3(std::format("pointLights[{}].specular", index), light->Color.ToVec3());
			shader.setFloat(std::format("pointLights[{}].constant", index), 1.0f);
			shader.setFloat(std::format("pointLights[{}].linear", index), 0.09f);
			shader.setFloat(std::format("pointLights[{}].quadratic", index), 0.032f);

			index += 1;
		}

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, lights[0]->depth_map);
		shader.setInt(std::format("shadowMaps[{}]", 0), 2);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, lights[1]->depth_map);
		shader.setInt(std::format("shadowMaps[{}]", 1), 3);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, lights[2]->depth_map);
		shader.setInt(std::format("shadowMaps[{}]", 2), 4);

		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, lights[3]->depth_map);
		shader.setInt(std::format("shadowMaps[{}]", 3), 5);
		// spotLight
		shader.setVec3("spotLight.position", camera.position);
		shader.setVec3("spotLight.direction", camera.front);
		shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		shader.setFloat("spotLight.constant", 1.0f);
		shader.setFloat("spotLight.linear", 0.09f);
		shader.setFloat("spotLight.quadratic", 0.032f);
		shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), (float)window.width / (float)window.height, 0.1f, 100.0f);
		glm::mat4 view = camera.ToView();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, GlobalPosition + Position);

		model = glm::rotate(model, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
		model = glm::rotate(model, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
		model = glm::rotate(model, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis

		model = glm::scale(model, glm::vec3(7.0f));
		shader.setMat4("model", model);

		{
			glm::mat4 lightProjection, lightView;
			glm::mat4 lightSpaceMatrix;
			float near_plane = 1.0f, far_plane = 7.5f;
			lightProjection = glm::ortho(-10.0f * 5, 10.0f * 5, -10.0f * 5, 10.0f * 5, NEAR_PLANE, FAR_PLANE);
			lightView = glm::lookAt(lights[0]->Position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
			lightSpaceMatrix = lightProjection * lightView;

			shader.setMat4("lightSpaceMatrix[0]", lightSpaceMatrix);
		}

		{
			glm::mat4 lightProjection, lightView;
			glm::mat4 lightSpaceMatrix;
			float near_plane = 1.0f, far_plane = 7.5f;
			lightProjection = glm::ortho(-10.0f * 5, 10.0f * 5, -10.0f * 5, 10.0f * 5, NEAR_PLANE, FAR_PLANE);
			lightView = glm::lookAt(lights[1]->Position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
			lightSpaceMatrix = lightProjection * lightView;

			shader.setMat4("lightSpaceMatrix[1]", lightSpaceMatrix);
		}

		{
			glm::mat4 lightProjection, lightView;
			glm::mat4 lightSpaceMatrix;
			float near_plane = 1.0f, far_plane = 7.5f;
			lightProjection = glm::ortho(-10.0f * 5, 10.0f * 5, -10.0f * 5, 10.0f * 5, NEAR_PLANE, FAR_PLANE);
			lightView = glm::lookAt(lights[2]->Position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
			lightSpaceMatrix = lightProjection * lightView;

			shader.setMat4("lightSpaceMatrix[2]", lightSpaceMatrix);
		}

		{
			glm::mat4 lightProjection, lightView;
			glm::mat4 lightSpaceMatrix;
			float near_plane = 1.0f, far_plane = 7.5f;
			lightProjection = glm::ortho(-10.0f * 5, 10.0f * 5, -10.0f * 5, 10.0f * 5, NEAR_PLANE, FAR_PLANE);
			lightView = glm::lookAt(lights[3]->Position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
			lightSpaceMatrix = lightProjection * lightView;

			shader.setMat4("lightSpaceMatrix[3]", lightSpaceMatrix);
		}

		this->model.Draw(shader);

		for (Object *object : children)
		{
			object->Rotation = Rotation;
			object->Position = Position;
			object->Render(window, camera, lights);
		}
	}
};
