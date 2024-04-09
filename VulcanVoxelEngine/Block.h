#pragma once
#include "CubeObject.h"

class Block {
public:
	Block();
	Block(CubeObject object);

private:
	CubeObject object;
};

