#pragma once
#include "Node.h"


struct Header {
	std::string blockID;
	unsigned int height;
	std::string merkleRoot;
	unsigned int nTx;
	unsigned int nonce;
	std::string prevBlockID;
};

class SPVNode :
	public Node
{
public:
	SPVNode();
	SPVNode(unsigned int ID_, std::string IP_, unsigned int port_);
	~SPVNode();

	void spvCallback(std::string msg);
	//Funciones para mensajes
	bool POSTFilter(unsigned int neighbourID);
	
	bool GETBlockHeader(unsigned int neighbourID, std::string& blockID_, unsigned int count);
	bool makeTransaction(unsigned int neighbourID, std::string& wallet, unsigned int amount);


	//Funciones para crear jsons.
	json createJSONTx(Transaction Tx_);
	json createJSONFilter(std::string Id_);

private:
};

