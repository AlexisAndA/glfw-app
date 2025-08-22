#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

Window::Window(int width, int height, std::string title)
{
	this->width = width;
	this->height = height;

	id = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (id == nullptr)
	{
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(id);
	glfwSetFramebufferSizeCallback(id, framebuffer_size_callback);

	for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
	{
		previous_key_states[key] = glfwGetKey(id, key);
	}
}

void Window::ProcessInput()
{
	if (glfwGetKey(id, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(id, true);
	}
}

void Window::Update()
{
	glfwSwapBuffers(id);
	glfwPollEvents();
}

void Window::ClearBackground(Graphics::Color color)
{
	glClearColor(color.r / 255.0, color.g / 255.0, color.b / 255.0, color.a / 255.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(id);
}

bool Window::IsKeyPressed(int key)
{
	return (glfwGetKey(id, key) == GLFW_PRESS);
}

bool Window::IsKeyJustPressed(int key)
{
	int currentKeyState = glfwGetKey(id, key);
	int previousKeyState = previous_key_states[key];

	// Update the previous state for the next frame
	previous_key_states[key] = currentKeyState;

	return (currentKeyState == GLFW_PRESS && previousKeyState == GLFW_RELEASE);
}

void Window::SetScrollCallback(GLFWscrollfun callback_function)
{
	glfwSetScrollCallback(id, callback_function);
}