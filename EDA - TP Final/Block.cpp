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

//void Block::getMerkleTree(void)
//{
//	uint numOfLeaves = pow(2, ceil(log((long double)ntx) / log(2))); //Busco la potencia de dos más cercana. Gracias StackOverflow por el código
//	for (int i = 0; i < ntx; i++)
//	{
//		uint numOfTransactions = TxVector[i].nTxin;
//		for (int j = 0; j < numOfTransactions; j++)
//		{
//			string newID;
//			uint tempID;
//			tempID = generateID((unsigned char*)(TxVector[i].vIn[j].txID).c_str());
//			newID = to_string(tempID);
//			//Convertir newID a HexaASCII
//			//Tree.Tree.push_back(newIDstr)
//			//Preguntar: se supone que el nodo del árbol sería concatenar todos los txid de Vin de un Tx[i]?
//			//Es decir, una transacción sería todo el conjunto de Vin, o cada uno de ese conjunto es una transacción diferente?
//		}
//	}
//}

static unsigned int generateID(unsigned char *str)
{
	unsigned int ID = 0;
	int c;
	while (c = *str++)
		ID = c + (ID << 6) + (ID << 16) - ID;
	return ID;
}
