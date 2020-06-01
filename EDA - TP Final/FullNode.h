#pragma once
#include "Node.h"


class FullNode :
	public Node
{
public:
	FullNode();
	FullNode(unsigned int ID_, std::string IP_, unsigned int port_);
	~FullNode();
	virtual bool addNeighbour(unsigned int ID_,std::string IP_, unsigned int port_);


	//Funciones para enviar mensajes.
	virtual bool POSTBlock(unsigned int neighbourID, std::string& BlockID);
	virtual bool POSTTransaction(unsigned int neighbourID, Transaction Tx_);
	virtual bool POSTMerkleBlock(unsigned int neighbourID, std::string BlockID_, std::string TxID);
	virtual bool GETBlocks(unsigned int neighbourID, std::string& blockID_, unsigned int count);

	//Funciones para dar respuestas
	virtual std::string POSTreply(std::string&receivedRequest, unsigned int clientPort_);
	virtual std::string GETreply(std::string&receivedRequest, unsigned int clientPort_);

	//Funciones para generar los JSON de los mensajes
	json createJSONBlock(std::string& BlockId);
	json createJSONTx(Transaction Tx_);
	json createJSONMerkleBlock(std::string BlockID_, std::string TxID);
	json createJSONHeader(std::string BlockID_);

private:
	Blockchain NodeBlockchain;
	std::vector <std::string> filters;
};

