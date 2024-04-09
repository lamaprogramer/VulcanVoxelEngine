#include "BasicObject.h"

BasicObject::BasicObject() {

}


BasicObject::BasicObject(glm::vec3 position, std::string modelName, std::string textureName) {
	this->worldPosition = position;
	this->instance = { Matricies::createModelMatrix(position, 0.5f, glm::radians(0.0), glm::vec3(0.0, 0.0, 1.0)) };
	this->modelName = modelName;
	this->textureName = textureName;
}
void BasicObject::updatePosition(glm::vec3 position) {
	this->worldPosition = position;
	this->instance.modelMatrix = Matricies::createModelMatrix(position, 0.5f, glm::radians(0.0), glm::vec3(0.0, 0.0, 1.0));
}