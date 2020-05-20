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

	int blockID;
	uint outputIndex;
	int signature;
	int transID;

} vinS;

typedef struct {

	uint amount;
	int publicID;

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

	//uint getBlockID();

private:

	int bigBlockID;
	uint height;
	int merkleRoot;
	uint ntx;
	int nonce;			//??
	int prevBlockID;
	transactions tx;

};

