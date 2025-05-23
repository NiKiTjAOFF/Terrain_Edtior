#ifndef MOUSEPICKER_H
#define MOUSEPICKER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../IO/Screen.h"
#include "../../IO/Mouse.h"

class MousePicker
{
	//Precision of point coordinates from mouse ray and terrain intersection
	static const int RECURSION_COUNT = 200;
	//Length of a mouse ray
	static constexpr float RAY_RANGE = 600.0f;

	//Find mouse ray
	//Translate mouse coordinates from sceen to clip space
	static glm::vec2 getNormalizedDeviceCoords();
	//Translate mouse coordinates from clip space to view space
	static glm::vec4 getViewCoords(glm::mat4 projectionMatrix, glm::vec4 clipCoords);
	//Translate mouse coordinates from view space to world space
	static glm::vec3 getWorldCoords(glm::mat4 viewMatrix, glm::vec4 viewCoords);

	//Find point of intersection of a mouse ray and a terrain
	glm::vec3 getPointOnRay(glm::vec3 ray, float distance);
	glm::vec3 binarySearch(int count, float start, float finish, glm::vec3 ray);
	bool intersectionInRange(float start, float finish, glm::vec3 ray);
	bool isUnderGround(glm::vec3 testPoint);

public:
	//Translate mouse coordinates from screen coordinates to world coordinates
	static glm::vec3 update(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
};

#endif