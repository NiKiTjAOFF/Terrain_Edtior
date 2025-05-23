#include "Camera.h"

Camera::Camera(glm::vec3 position)
	: cameraPos(position),
	worldY(glm::vec3(0.0f, 1.0f, 0.0f)),
	yaw(0.0f),
	pitch(0.0f),
	speed(10.0f),
	zoom(45.0f),
	shouldMoveRelativeToWorldYAxis(true),
	cameraFront(glm::vec3(0.0f, 0.0f, 0.0f))
{
	updateCameraVectors();
}

void Camera::updateCameraDirection(double dx, double dy)
{
	yaw += dx;
	pitch += dy;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	else if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
	
	updateCameraVectors();
}
void Camera::updateCameraPos(CameraDirection direction, double dt)
{
	float velocity = (float)dt * speed;
	glm::vec3 vectorX = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
	glm::vec3 vectorY = glm::normalize(glm::vec3(0.0f, cameraUp.y, 0.0f));
	glm::vec3 vectorZ = glm::normalize(glm::vec3(cameraRight.x, 0.0f, cameraRight.z));

	if (direction == CameraDirection::FORWARD)
	{
		cameraPos += vectorX * velocity;
	}
	else if (direction == CameraDirection::BACKWARD)
	{
		cameraPos -= vectorX * velocity;
	}
	else if (direction == CameraDirection::RIGHT)
	{
		cameraPos += vectorZ * velocity;
	}
	else if (direction == CameraDirection::LEFT)
	{
		cameraPos -= vectorZ * velocity;
	}
	else if (direction == CameraDirection::UP)
	{
		cameraPos += vectorY * velocity;
	}
	else if (direction == CameraDirection::DOWN)
	{
		cameraPos -= vectorY * velocity;
	}
}
void Camera::updateCameraZoom(double dy)
{
	if (zoom >= 1.0f && zoom <= 45.0f)
	{
		zoom -= dy;
	}
	else if (zoom < 1.0f)
	{
		zoom = 1.0f;
	}
	else // > 45.0f
	{
		zoom = 45.0f;
	}
}

float Camera::getZoom()
{
	return zoom;
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::updateCameraVectors()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

	cameraRight = glm::normalize(glm::cross(cameraFront, worldY));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}