#pragma once
#include "Vertex.h"

struct Model {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};