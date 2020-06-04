#include "Block.h"
#include <iostream>


Block::Block() {

}

Block::Block(const json & j)
{
	jsonBlock = j;

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


void Block::setPush_Back(Transaction tx_)
{
	TxVector.push_back(tx_);
}

void Block::createMerkleLeaves(void)
{

	string concatenate = "";
	string newID="";
	uint ID=0;
	Tree.numberOfLeaves = pow(2, ceil(log((long double)ntx) / log(2))); 
	Tree.height = (uint)log2(Tree.numberOfLeaves);

	for (int i = 0; i < ntx; i++)
	{	
		concatenate = "";
		string txID="";
		for (int j = 0; j < TxVector[i].nTxin; j++)
		{
			concatenate += TxVector[i].vIn[j].txID;
		}
		ID = generateID((unsigned char*)concatenate.c_str());


		//sprintf copia el número ID pasado a Hexa y que ocupe 8 lugares y lo guarda en tohex
		char tohex[50];
		int n = sprintf_s(tohex, sizeof(tohex), "%08X", ID);
		string newIDstr(tohex);
		
		Tree.Tree.push_back(newIDstr);
		Tree.EntireTree.push_back(newIDstr);			//Guardo hojas en real

		if ((i == ntx - 1) && ((ntx % 2) != 0)) {

			Tree.Tree.push_back(newIDstr);
			Tree.EntireTree.push_back(newIDstr);			//Guardo hojas en real
		}
	}

	stringMerkleRoot = Tree.Tree;
	Tree.Tree.clear();					//Limpio el Tree luego de guardarlo en MerkleRoot
}



//recurs

void Block::generateMerkleRoot(vector<string>& stringMerkleRoot)
{

	vector<string> temp;
	temp.clear();
	auto sz = stringMerkleRoot.size();


	if (sz == 1) {

		return;

	}
	else {

		for (int i = 0; i < sz; i += 2) {

			string line;
			if ((i == sz - 1) && ((sz % 2) != 0)) {

				line = stringMerkleRoot[i] + stringMerkleRoot[i ];

			}
			else {

				line = stringMerkleRoot[i] + stringMerkleRoot[i + 1];
			}

			unsigned int ID = generateID((unsigned char*)(line.c_str()));

			char tohex[9];
			int n = sprintf_s(tohex, sizeof(tohex), "%08X", ID);
			string newIDstr(tohex);

			temp.push_back(newIDstr);
			Tree.EntireTree.push_back(newIDstr);
		}
		stringMerkleRoot = temp;
		generateMerkleRoot(stringMerkleRoot);
	}

}

string Block::getCalculatedMerkleRoot(void)
{
	return Tree.merkleRoot;
}

bool Block::createMerkleTree(void){

	createMerkleLeaves();
	generateMerkleRoot(stringMerkleRoot);

	Tree.merkleRoot = stringMerkleRoot[0];

	stringMerkleRoot.clear();
	if (getMerkleRoot() == Tree.merkleRoot) {
		return true;
	}
	else {
		return false;
	}
}

vector<string> Block::getMerklePath(Transaction Tx_)
{
	vector <string> path;
	path.clear();
	auto tx_ = TxVector.begin();
	for (int i = 0; i < TxVector.size(); i++)
	{
		if (tx_->txID == Tx_.txID)
			break;
		tx_++;
	}

	//Si se encontró el Id dentro de la transacción
	if (tx_ != TxVector.end())
	{
		int j = 0;
		while (j < (Tree.EntireTree.size()-1))
		{
			if (j % 2)
				path.push_back(Tree.EntireTree[--j]);
			else
				path.push_back(Tree.EntireTree[j + 1]);
			j = j / 2 + pow(2, log2(Tree.EntireTree.size() + 1) - 1);
		}
	}

	return path;
}

MerkleTree Block::getMerkleTree(void)
{
	return Tree;
}

vector<string> Block::getNodos(void)
{
	return Tree.EntireTree;
}

uint Block::getMerkleHeight(void)
{
	return Tree.height;
}

uint Block::getNumLeaves(void)
{
	return Tree.numberOfLeaves;
}

static unsigned int generateID(unsigned char *str)
{
	unsigned int ID = 0;
	int c;
	while (c = *str++)
		ID = c + (ID << 6) + (ID << 16) - ID;
	return ID;
}
