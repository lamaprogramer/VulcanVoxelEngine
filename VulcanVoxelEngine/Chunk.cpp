#include "Chunk.h"

Chunk::Chunk() {}
Chunk::Chunk(int x, int y, int z) {
	chunk = std::vector(x, 
		std::vector<std::vector<Block>>(y, 
			std::vector<Block>(z)
		)
	);
}

void Chunk::setBlock(glm::ivec3 pos, Block block) {
	chunk[pos.x][pos.y][pos.z] = block;
}

Block Chunk::getBlock(glm::ivec3 pos) {
	return chunk[pos.x][pos.y][pos.z];
}