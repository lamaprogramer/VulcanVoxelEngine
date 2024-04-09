#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Block.h"

class Chunk {
public:
	Chunk();
	Chunk(int x, int y, int z);

	Block getBlock(glm::ivec3 pos);
	void setBlock(glm::ivec3 pos, Block block);

private:
	std::vector<std::vector<std::vector<Block>>> chunk;
};

