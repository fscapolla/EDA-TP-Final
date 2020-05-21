#include "Block.h"
Block::Block()
{
	//NOSE SI PONER Q UN CONSTRUCTOR DE BLOCK RECIBA EL PATH Y GUARDE TODA LA INFO 
	//O HACERLO TODO EN GRAPHIC
}


/* SETTERS */

void Block::setbigBlockID(string BBID_)
{
	bigBlockID = BBID_;
}


void Block::setmerkleRoot(string merkle_)
{
	merkleRoot = merkle_;
}

void Block::setntx(uint ntx_)
{
	ntx = ntx_;
}

void Block::setNonce(int nonce_)
{
	nonce = nonce_;
}

void Block::setprevBlockID(string PBID_)
{
	prevBlockID = PBID_;
}

void Block::setTX(transactions tx_)
{
	tx = tx_;
}

/* GETTERS */
string Block::getTXBID(uint i)
{
	return tx.vin[i].blockID;
}

string Block::getBBID()
{
	return bigBlockID;
}
