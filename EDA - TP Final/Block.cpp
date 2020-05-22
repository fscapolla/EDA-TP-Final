#include "Block.h"
#include <iostream>

Block::Block(const json & j)
{
	/*Cargo los datos del bloque*/
	height = j["height"].get<unsigned int>();
	BigBlockID = j["blockid"].get<string>();
	prevBlockID = j["previousblockid"].get<string>();
	ntx = j["nTx"].get<unsigned int>();
	nonce = j["nonce"].get<unsigned int>();

	//Cargo el arreglo de transacciones
	auto TxInfo= j["tx"];
	for (auto& tx_ : TxInfo)
	{
		Transaction tempTx;
		auto nTxin_ = tx_["nTxin"].get<unsigned int>();
		tempTx.nTxin = nTxin_;
		auto nTxout_ = tx_["nTxout"].get<unsigned int>();
		tempTx.nTxout = nTxout_;
		auto txID_ = tx_["txid"].get<string>();
		tempTx.txID = txID_;
		auto Vin_ = tx_["vin"];
		for (auto& vin_ : Vin_)
		{
			VinS tempVin;

			auto LilBlockId_ = vin_["blockid"].get<string>();
			tempVin.LilblockID = LilBlockId_;
			auto outputIndex_ = vin_["outputIndex"].get<int>();
			tempVin.outputIndex = outputIndex_;
			auto signature_ = vin_["signature"].get<string>();
			tempVin.signature = signature_;
			auto txid_ = vin_["txid"].get<string>();
			tempVin.txID = txid_;

			tempTx.vIn.push_back(tempVin);
		}
		auto Vout_ = tx_["vout"];
		for (auto& vout_ : Vout_)
		{
			VoutS tempVout;

			auto amount_ = vout_["amount"].get<unsigned int>();
			tempVout.amount = amount_;
			auto publicID_ = vout_["publicid"].get<string>();
			tempVout.publicID = publicID_;

			tempTx.vOut.push_back(tempVout);
		}

		TxVector.push_back(tempTx);
	}
}

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
	string concatenate = "";
	string newID="";
	uint ID=0;
	uint numOfLeaves = pow(2, ceil(log((long double)ntx) / log(2))); 
	
	for (int i = 0; i < ntx; i++)
	{
		string txID="";
		for (int j = 0; j < TxVector[i].nTxin; j++)
		{
			concatenate += TxVector[i].vIn[j].txID;
		}
		/*ID = generateID((unsigned char*)concatenate.c_str());
		txID = to_string(ID);*/
		/*while (txID.size() < IDSIZE) {
			txID = "0" + txID;
		}*/

		//sprintf copia el número ID pasado a Hexa y que ocupe 8 lugares y lo guarda en tohex
		char tohex[9];
		sprintf_s(tohex, sizeof(tohex), "%08X", ID);
		string newIDstr(tohex);
		
		Tree.Tree.push_back(tohex);
		
		if ((i = ntx - 1) && ntx % 2 != 0) {

			Tree.Tree.push_back(tohex);

		}
	}
	stringMerkleRoot = Tree.Tree;
	
}



//recurs

void Block::generateMerkleRoot(vector<string>& stringMerkleRoot)
{

	vector<string> temp;
	auto sz = temp.size();
	temp.clear();


	for (int i = 0;i < sz;i++) {

		string line;
		line = stringMerkleRoot[i] + stringMerkleRoot[i];

		unsigned int ID = generateID((unsigned char*)(line.c_str()));
		line = to_string(ID);
		while (line.size() < IDSIZE) { //No sería <= ?
			line = "0" + line;
		}

		temp.push_back(line);

	}

	stringMerkleRoot = temp;

	if (stringMerkleRoot.size() == 1) {

		return;

	}
	else {
		generateMerkleRoot(stringMerkleRoot);
	}


}



bool Block::createMerkleTree(void){

	createMerkleLeaves();
	generateMerkleRoot(stringMerkleRoot); //Completa los nodos internos del MerkleTree?
	Tree.merkleRoot = stringMerkleRoot[0];
	if (getMerkleRoot() == Tree.merkleRoot) {
		return true;
	}
	else {
		return false;
	}
}

void Block::printBlockInfo(void)
{
	cout << BigBlockID << endl;
	cout << height << endl;
	cout << merkleRoot << endl;
	cout << ntx << endl;
	cout << nonce << endl;
	cout << prevBlockID << endl;
}




static unsigned int generateID(unsigned char *str)
{
	unsigned int ID = 0;
	int c;
	while (c = *str++)
		ID = c + (ID << 6) + (ID << 16) - ID;
	return ID;
}
