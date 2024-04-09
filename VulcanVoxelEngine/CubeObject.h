#pragma once

#include "BasicObject.h"

class CubeObject: public BasicObject {
public:
    static std::vector<Vertex> cubeVertices;
    static std::vector<uint32_t> cubeIndices;

	CubeObject();
	CubeObject(glm::vec3 position, std::string modelName, std::string textureName);
};