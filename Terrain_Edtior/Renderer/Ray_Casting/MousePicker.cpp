#include "MousePicker.h"
#include <iostream>

glm::vec3 MousePicker::update(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	//Find mouse ray
	glm::vec2 normalizedCoords = getNormalizedDeviceCoords();
	//Ray is pointing into the screen, so z-axis = -1.0f
	glm::vec4 clipCoords = { normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f };
	glm::vec4 viewCoords = getViewCoords(projectionMatrix, clipCoords);
	glm::vec3 worldCoords = getWorldCoords(viewMatrix, viewCoords);
	
	return glm::normalize(worldCoords);

	//TODO: MousePicker object placement point search
	//Find point of intersection of a mouse ray and a terrain
	/*if (intersectionInRange(0, RAY_RANGE, worldCoords))
	{
		return binarySearch(0, 0, RAY_RANGE, worldCoords);
	}
	else
	{
		return glm::vec3(0.0f);
	}*/

}

glm::vec2 MousePicker::getNormalizedDeviceCoords()
{
	//Mouse coords to OpenGL screen coords [-1.0f, 1.0f]
	float x = (2.0f * Mouse::getMouseX()) / Screen::s_width - 1.0f;
	float y = (2.0f * Mouse::getMouseY()) / Screen::s_height - 1.0f;
	return glm::vec2(x, y);
}

glm::vec4 MousePicker::getViewCoords(glm::mat4 projectionMatrix, glm::vec4 clipCoords)
{
	glm::mat4 invertedProjection = glm::inverse(projectionMatrix);
	glm::vec4 viewCoords = invertedProjection * clipCoords;
	//Ray is pointing into the screen, so z-axis = -1.0f
	return glm::vec4(viewCoords.x, viewCoords.y, -1.0f, 0.0f);
}

glm::vec3 MousePicker::getWorldCoords(glm::mat4 viewMatrix, glm::vec4 viewCoords)
{
	glm::mat4 invertedView = glm::inverse(viewMatrix);
	glm::vec4 worldCoords = invertedView * viewCoords;
	return glm::vec3(worldCoords.x, worldCoords.y, worldCoords.z);
}


//TODO: Mouse Picker objtc placement point search additional functions
//glm::vec3 MousePicker::getPointOnRay(glm::vec3 ray, float distance)
//{
//
//}
//glm::vec3 MousePicker::binarySearch(int count, float start, float finish, glm::vec3 ray)
//{
//
//}
//bool MousePicker::intersectionInRange(float start, float finish, glm::vec3 ray)
//{
//
//}
//bool MousePicker::isUnderGround(glm::vec3 testPoint)
//{
//	
//}