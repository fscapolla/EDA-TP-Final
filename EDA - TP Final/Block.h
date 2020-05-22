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
//typedef string newIDstr;
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

	/*
	Transaction() {
		nTxin = 0;
		nTxout = 0;
		txID = "";
	}

	Transaction(uint nTxin_, uint nTxout_, string txID_, VinS& VinS_, VoutS& VoutS_) {
		nTxin = nTxin_;
		nTxout = nTxout_;
		txID = txID_;
		VinS (VinS_);
		VoutS (VoutS_);
	}
	*/
};

struct MerkleTree {
	uint height;
	string merkleRoot;
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

	string getBlockID(void);
	uint getHeight(void);
	string getMerkleRoot(void);
	uint getNtx(void);
	uint getNonce(void);
	string getPrevBlovkID(void);
	vector<Transaction>& getTxVector();

	void setBlockID(string blockID_);
	void setHeight(uint height_);
	void setMerkleRoot(string merkleRoot_);
	void setNtx(uint ntx_);
	void setNonce(uint nonce_);
	void setPrevBlockID(string prevBlockID_);

	void setTX(Transaction);

	bool validateMerkleRoot(string MerkleRoot_);

	void createMerkleLeaves(void);
	
	void generateMerkleRoot(vector<string>& myVec);

	bool createMerkleTree();
	string getCalcMR() { return Tree.merkleRoot; }

	void printBlockInfo(void);

private:

	string BigBlockID;
	uint height;
	string merkleRoot;
	uint ntx;
	uint nonce;			//??
	string prevBlockID;
	MerkleTree Tree;
	vector<string> stringMerkleRoot; //String usado para llenar el árbol?
	vector<Transaction> TxVector; //No necesitamos un vector de transacciones
	Transaction tx; 

};

