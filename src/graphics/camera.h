#pragma once

#include <glm/glm.hpp>
#include "window.h"

constexpr float SENSIVITY = 100.0f;
constexpr float BASE_SPEED = 5.0f;

constexpr glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

enum class CameraMode
{
	WALKING,
	FLYING,
};

class Camera
{
	glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 camera_direction = glm::normalize(position - camera_target);
	float yaw = -90.0f;
	float pitch = 0.0f;

	CameraMode camera_mode = CameraMode::WALKING;

	void HandleModeToggle(Window window);
	void HandleFlying(Window window, float speed);
	void HandleWalking(Window window, float speed);

public:
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.5f);

	Camera(glm::vec3 position = glm::vec3(0.0f));

	void ClampPitch();

	void ProcessInput(Window window, float delta_time);

	glm::mat4 ToView();

	float GetFov();
};