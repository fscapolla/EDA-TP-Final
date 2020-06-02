#pragma once
#include "Node.h"


class FullNode :
	public Node
{
public:
	FullNode();
	FullNode(unsigned int ID_, std::string IP_, unsigned int port_);
	~FullNode();

	void fullCallback(std::string msg);
	//Funciones para enviar mensajes.
	bool POSTBlock(unsigned int neighbourID, std::string& BlockID);
	bool POSTMerkleBlock(unsigned int neighbourID);
	bool GETBlocks(unsigned int neighbourID, std::string& blockID_, unsigned int count);

	

	//Funciones para generar los JSON de los mensajes
	json createJSONBlock(std::string& BlockId);
	json createJSONTx(Transaction Tx_);
	json createJSONMerkleBlock(void);

	//virtual bool performRequest(void); Implementada en Node.h

private:
	Blockchain NodeBlockchain;
	std::vector <std::string> filters;
};

