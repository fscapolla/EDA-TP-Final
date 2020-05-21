#pragma once
#include "Block.h"
#include <list>

class Blockchain
{
public:
	list<Block>& getBchain(void) { return Bchain; }

private:
	list<Block> Bchain;

};

