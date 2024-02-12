#include "Matrices.h"

glm::mat4 Matricies::createModelMatrix(glm::vec3 translation, float scaleFactor, float angleInRadians, glm::vec3 rotationAxis) {
	glm::mat4 modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, translation);
	modelMatrix = glm::rotate(modelMatrix, angleInRadians, rotationAxis);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));

	return modelMatrix;
}

glm::mat4 Matricies::createPerspectiveMatrix(float fovInRadians, float aspectRatio) {
	return glm::perspective(
		fovInRadians,
		aspectRatio,
		0.1f,
		100.0f
	);
}