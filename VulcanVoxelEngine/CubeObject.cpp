#include "CubeObject.h"

std::vector<Vertex> CubeObject::cubeVertices = {
		{{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // 0
		{{1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // 1
		{{1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}, // 2
		{{-1.0f,  1.0f,  1.0f}, { 1.0f, 0.0f, 0.0f }, {0.0f, 0.0f}}, // 3

		{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} }, // 4
		{{1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} }, // 5
		{{1.0f,  1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} }, // 6
		{{-1.0f,  1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} } // 7
};

std::vector<uint32_t> CubeObject::cubeIndices = {
		0,1,2, 2,3,0,
		1,5,6, 6,2,1,
		7,6,5, 5,4,7,
		4,0,3, 3,7,4,
		4,5,1, 1,0,4,
		3,2,6, 6,7,3
};

CubeObject::CubeObject() {}
CubeObject::CubeObject(glm::vec3 position, std::string modelName, std::string textureName) :
	BasicObject{
		position,
		modelName,
		textureName
	} {}