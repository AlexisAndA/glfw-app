#pragma once

#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>
#include "color.h"
#include <map>

class Window
{
private:
	std::map<int, int> previous_key_states;
	GLFWwindow *id;

public:
	int width;
	int height;

	Window(int width = 1280, int height = 720, std::string title = "Engine");

	void ProcessInput();

	void Update();

	void ClearBackground(Graphics::Color color);

	bool ShouldClose();

	bool IsKeyPressed(int key);
	bool IsKeyJustPressed(int key);

	void SetScrollCallback(GLFWscrollfun callback_function);
};