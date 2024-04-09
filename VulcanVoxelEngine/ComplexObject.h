#pragma once
#include "BasicObject.h"

class ComplexObject: public BasicObject {
public:

	ComplexObject();
	ComplexObject(glm::vec3 position, std::string modelPath, std::string texturePath);

	void loadModel(std::string modelPath);
};