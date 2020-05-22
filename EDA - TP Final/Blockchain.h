#pragma once
#include "Block.h"
#include "json.hpp"
#include <list>

using json = nlohmann::json;

class Blockchain
{
public:
	list<Block>& getBchain(void) { return Bchain; }
	void parsingBlockchain(json chain_JData);
	vector<Block>& getBlocksArr(void) { return BlocksArr; }

private:

	list<Block> Bchain;
	vector<Block> BlocksArr;

};

