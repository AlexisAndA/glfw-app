#include "camera.h"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <glm/ext/matrix_transform.hpp>

float fov = 90.0f;

void Camera::HandleModeToggle(Window window)
{
	if (window.IsKeyJustPressed(GLFW_KEY_LEFT_CONTROL))
	{
		if (camera_mode == CameraMode::FLYING)
		{
			camera_mode = CameraMode::WALKING;
		}
		else
		{
			camera_mode = CameraMode::FLYING;
		}
	}
}

void Camera::HandleFlying(Window window, float speed)
{
	if (window.IsKeyPressed(GLFW_KEY_W))
	{
		position += speed * front;
	}
	if (window.IsKeyPressed(GLFW_KEY_S))
	{
		position -= speed * front;
	}
	if (window.IsKeyPressed(GLFW_KEY_A))
	{
		position -= glm::normalize(glm::cross(front, UP)) * speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_D))
	{
		position += glm::normalize(glm::cross(front, UP)) * speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_X))
	{
		position.y -= speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_Z))
	{
		position.y += speed;
	}
}

void Camera::HandleWalking(Window window, float speed)
{
	// Project the front vector onto the horizontal plane for walking
	glm::vec3 horizontal_front =
		glm::normalize(glm::vec3(front.x, 0.0f, front.z));
	glm::vec3 horizontal_right =
		glm::normalize(glm::cross(horizontal_front, UP));

	if (window.IsKeyPressed(GLFW_KEY_W))
	{
		position += speed * horizontal_front;
	}
	if (window.IsKeyPressed(GLFW_KEY_S))
	{
		position -= speed * horizontal_front;
	}
	if (window.IsKeyPressed(GLFW_KEY_A))
	{
		position -= horizontal_right * speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_D))
	{
		position += horizontal_right * speed;
	}
	// In WALKING mode, Z and X keys should not directly change y-position
	// If you had a physics engine, gravity would handle vertical movement
}

Camera::Camera(glm::vec3 position) : position(position) {}

void Camera::ProcessInput(Window window, float delta_time)
{
	HandleModeToggle(window);

	float speed = BASE_SPEED * delta_time;
	if (window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		speed *= 10.0f;
	}

	float turn_speed = SENSIVITY * delta_time;

	// Calculate new front vector based on yaw and pitch
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);

	// Movement logic based on camera mode
	if (camera_mode == CameraMode::FLYING)
	{
		HandleFlying(window, speed);
	}
	else if (camera_mode == CameraMode::WALKING)
	{
		HandleWalking(window, speed);
	}

	// Yaw and Pitch (turning) are the same for both modes
	if (window.IsKeyPressed(GLFW_KEY_LEFT))
	{
		yaw -= turn_speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_RIGHT))
	{
		yaw += turn_speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_UP))
	{
		pitch += turn_speed;
	}
	if (window.IsKeyPressed(GLFW_KEY_DOWN))
	{
		pitch -= turn_speed;
	}

	ClampPitch();
}

void Camera::ClampPitch()
{
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
}

glm::mat4 Camera::ToView()
{
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(position, position + front, UP);
	// view = glm::rotate(view, glm::radians(90.0f), rotation);
	return view;
}

float Camera::GetFov()
{
	return fov;
}