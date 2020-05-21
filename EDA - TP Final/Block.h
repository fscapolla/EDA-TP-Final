#pragma once
#include<vector>
#include<string>
#include <list> 
#include <iterator>
#include <stdio.h>
#include <iostream>


#define MAX_TX 12
typedef unsigned int uint;
using namespace std;

typedef struct {

	string blockID;
	uint outputIndex;
	int signature;
	int transID;			//NO ESTOY SEGURA DEL TIPO

} vinS;

typedef struct {

	uint amount;
	string publicID;

} voutS;

typedef struct {

	uint nTxin;
	uint nTxout;
	int txID;
	vector<vinS> vin;
	vector<voutS> vout;

} transactions;


class Block
{
public:
	Block();
	/* setters */
	void setbigBlockID(string);
	void setmerkleRoot(string);
	void setntx(uint);
	void setNonce(int);
	void setprevBlockID(string);
	void setTX(transactions);

	/* getters */
	string getTXBID(uint i);	// get Transaction Block ID
	string getBBID();			// get Big Block ID


private:

	string bigBlockID;
	uint height;
	string merkleRoot;
	uint ntx;
	int nonce;			//??
	string prevBlockID;
	transactions tx;

};

