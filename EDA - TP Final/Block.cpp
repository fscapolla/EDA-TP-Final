#include "Block.h"

string Block::getBlockID(void)
{
	return BigBlockID;
}

uint Block::getHeight(void)
{
	return height;
}

string Block::getMerkleRoot(void)
{
	return merkleRoot;
}

uint Block::getNtx(void)
{
	return ntx;
}

uint Block::getNonce(void)
{
	return nonce;
}

string Block::getPrevBlovkID(void)
{
	return prevBlockID;
}

vector<Transaction>& Block::getTxVector()
{
	return TxVector;
}

void Block::setBlockID(string blockID_)
{
	BigBlockID = blockID_;
}

void Block::setHeight(uint height_)
{
	height = height_;
}

void Block::setMerkleRoot(string merkleRoot_)
{
	merkleRoot = merkleRoot_;
}

void Block::setTX(Transaction tx_)
{
	tx = tx_;
}

void Block::setNtx(uint ntx_)
{
	ntx = ntx_;
}

void Block::setNonce(uint nonce_)
{
	nonce = nonce_;
}

void Block::setPrevBlockID(string prevBlockID_)
{
	prevBlockID = prevBlockID_;
}

bool Block::validateMerkleRoot(string MerkleRoot_)
{
	if (merkleRoot == MerkleRoot_) {
		return true;
	}
	else return false;
}


void Block::createMerkleLeaves(void)
{
	//uint numOfLeaves = pow(2, ceil(log((long double)ntx) / log(2))); //Busco la potencia de dos más cercana. Gracias StackOverflow por el código
	
	for (int i = 0; i < ntx; i++)
	{
		string txID="";
		for (int j = 0; j < TxVector[i].nTxin; j++)
		{
			string newID;
			uint tempID;
			tempID = generateID((unsigned char*)(TxVector[i].vIn[j].txID).c_str());
			newID = to_string(tempID);
			while (newID.size() < IDSIZE) {
				newID = "0" + newID;
			}
			txID += newID;
		}
		

		unsigned int ID = generateID((unsigned char*)(txID.c_str()));
		txID = to_string(ID);
		while (txID.size() < IDSIZE) {
			txID = "0" + txID;
		}
		
		Tree.Tree.push_back(txID);
		
		if ((i = ntx - 1) && ntx % 2 != 0) {

			Tree.Tree.push_back(txID);

		}
		
		
	}

	
}

static unsigned int generateID(unsigned char *str)
{
	unsigned int ID = 0;
	int c;
	while (c = *str++)
		ID = c + (ID << 6) + (ID << 16) - ID;
	return ID;
}
