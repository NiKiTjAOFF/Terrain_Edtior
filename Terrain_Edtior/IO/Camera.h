#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraDirection
{
	NONE = 0,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
	void updateCameraVectors();
public:
	glm::vec3 cameraPos;

	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;

	glm::vec3 worldY;

	float yaw;
	bool invertX;
	float pitch;
	bool invertY;
	float speed;
	float zoom;// -> FOV
	float mouseSensitivity;
	float wheelSensitivity;

	Camera(glm::vec3 position);

	void updateCameraDirection(double dx, double dy);
	void updateCameraPos(CameraDirection dir, double dt);
	void updateCameraZoom(double dy);

	float getZoom();

	glm::mat4 getViewMatrix();

};

#endif