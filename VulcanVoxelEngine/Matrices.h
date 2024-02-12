#pragma once
#include <glm/gtc/matrix_transform.hpp> 
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

class Matricies {
public:
	static glm::mat4 createModelMatrix(glm::vec3 translation, float scaleFactor, float angleInRadians, glm::vec3 rotationAxis);
	static glm::mat4 createPerspectiveMatrix(float fovInRadians, float aspectRatio);
};