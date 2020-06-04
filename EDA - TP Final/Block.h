#pragma once
#include <vector>
#include <string>
#include <list> 
#include <iterator>
#include <cstdio>
#include <iostream>
#include <cmath>
#include "json.hpp"

#define IDSIZE 8
#define MAX_TX 12
typedef unsigned long int uint;
using namespace std;

using json = nlohmann::json;

static unsigned int generateID(unsigned char *str);

struct VinS {

	string LilblockID;
	uint outputIndex;
	string signature;
	string txID;
};

struct VoutS {

	uint amount;
	string publicID;
};

struct Transaction {

	uint nTxin;
	uint nTxout;
	string txID;
	vector<VinS> vIn;
	vector<VoutS> vOut;

	
};


struct MerkleBlock {
	string blockID;
	vector<Transaction> tx;
	unsigned int txPos;
	vector<string> merklePath;
};

struct MerkleTree {
	uint height;
	string merkleRoot;
	vector<string> EntireTree;
	vector<string> Tree;


	uint numberOfLeaves;

	MerkleTree() {
		height = 0;
		merkleRoot = "";
		numberOfLeaves = 0;
		Tree.clear();
	}

	MerkleTree(const MerkleTree& MerkleTree_) : height(MerkleTree_.height), merkleRoot(MerkleTree_.merkleRoot), 
		numberOfLeaves(MerkleTree_.numberOfLeaves), Tree(MerkleTree_.Tree) {};
};

class Block
{
public:
	Block(const json& j);
	Block();
	string getBlockID(void);
	uint getHeight(void);
	string getMerkleRoot(void);
	uint getNtx(void);
	uint getNonce(void);
	string getPrevBlovkID(void);
	string getCalculatedMerkleRoot(void);
	vector<Transaction>& getTxVector();
	MerkleTree getMerkleTree(void);
	vector<string> getNodos(void);
	uint getMerkleHeight(void);
	uint getNumLeaves(void);

	void setBlockID(string blockID_);
	void setHeight(uint height_);
	void setMerkleRoot(string merkleRoot_);
	void setNtx(uint ntx_);
	void setNonce(uint nonce_);
	void setPrevBlockID(string prevBlockID_);
	void setPush_Back(Transaction tx_);

	void setTX(Transaction);

	bool validateMerkleRoot(string MerkleRoot_);

	void createMerkleLeaves(void);
	
	void generateMerkleRoot(vector<string>& myVec);

	bool createMerkleTree();

	//Función para recuperar el Merkle Path (agregada en fase 2).
	vector <string> getMerklePath(Transaction Tx_);
	

private:

	string BigBlockID;
	uint height;
	string merkleRoot;
	uint ntx;
	uint nonce;			
	string prevBlockID;
	MerkleTree Tree;
	vector<string> stringMerkleRoot; //String usado para llenar el árbol?
	vector<Transaction> TxVector; 
	Transaction tx; 
	json jsonBlock;

};

