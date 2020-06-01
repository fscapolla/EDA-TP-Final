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

	//Función para agregar un vecino
	virtual bool addNeighbour(unsigned int neighbourID, std::string IP_, unsigned int port_);

	//Funciones para mensajes
	virtual bool POSTFilter(unsigned int neighbourID);
	virtual bool POSTTransaction(unsigned int neighbourID, Transaction Tx_);
	virtual bool GETBlockHeader(unsigned int neighbourID, std::string& blockID_, unsigned int count);
	virtual bool makeTransaction(unsigned int neighbourID, std::string& wallet, unsigned int amount);

	//Funciones para dar respuestas
	virtual std::string POSTreply(std::string&receivedRequest, unsigned int clientPort);
	virtual std::string GETreply(std::string&receivedRequest, unsigned int clientPort);

	//Funciones para crear jsons.
	json createJSONTx(Transaction Tx_);
	json createJSONFilter(std::string Id_);

private:
};

