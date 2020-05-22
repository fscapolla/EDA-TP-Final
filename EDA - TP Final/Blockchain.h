#pragma once
#include "Block.h"
#include "json.hpp"
#include <list>
#include <fstream>
#define ERROR 4
using json = nlohmann::json;

class Blockchain
{
public:
	//list<Block>& getBchain(void) { return Bchain; }
	void parsingBlockchain(json chain_JData);
	vector<Block>& getBlocksArr(void) { return BlocksArr; }
	int saveBlockInfo(std::string);
private:

	//list<Block> Bchain;

	/* JSON */
	json Jdata;
	vector<Block> BlocksArr;

};

