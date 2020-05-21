#pragma once
#include <vector>
#include <string>
#include <list> 
#include <iterator>
#include <cstdio>
#include <iostream>
#include <cmath>


#define MAX_TX 12
typedef unsigned long int uint;
typedef string newIDstr;
using namespace std;

static unsigned int generateID(unsigned char *str);

struct VinS {

	string blockID;
	uint outputIndex;
	string signature;
	string txID;

	VinS() {
		blockID = "";
		outputIndex = 0;
		signature = "";
		txID = "";
	}

	VinS(const VinS& VinS_) {
		blockID = VinS_.blockID;
		outputIndex = VinS_.outputIndex;
		signature = VinS_.signature;
		txID = VinS_.txID;
	}

};

struct VoutS {

	uint amount;
	string publicID;

	VoutS() {
		amount = 0;
		publicID = "";
	}

	VoutS(const VoutS& VoutS_) {
		amount = VoutS_.amount;
		publicID = VoutS_.publicID;
	}

};

struct Transaction {

	uint nTxin;
	uint nTxout;
	string txID;
	vector<VinS> vIn;
	vector<VoutS> vOut;

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
};

struct MerkleTree {
	uint height;
	string merkleRoot;
	vector<string> Tree;

	MerkleTree() {
		height = 0;
		merkleRoot = "";
		Tree.clear();
	}

	MerkleTree(const MerkleTree& MerkleTree_) : height(MerkleTree_.height), merkleRoot(MerkleTree_.merkleRoot), Tree(MerkleTree_.Tree) {};
};

class Block
{
public:

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

	bool validateMerkleRoot(string MerkleRoot_);

	/*void getMerkleTree(void);*/


private:

	string BigBlockID;
	uint height;
	string merkleRoot;
	uint ntx;
	uint nonce;			//??
	string prevBlockID;
	MerkleTree Tree;
	vector<Transaction> TxVector;

};

