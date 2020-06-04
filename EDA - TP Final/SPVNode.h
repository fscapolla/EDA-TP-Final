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

	std::string getKey(void);
	void setKey(std::string Key_);


	json SpvCallback(std::string message);
	//Funciones para mensajes
	bool POSTFilter(unsigned int neighbourID, std::string key);
	
	bool GETBlockHeader(unsigned int neighbourID, std::string& blockID_, unsigned int count);
	bool makeTransaction(unsigned int neighbourID, std::string& wallet, unsigned int amount);

	//Funciones para generar los JSON de los mensajes

	json createJSONFilter(std::string Id_);



private:
	json findMBlockJSON(std::string message);
	std::string publickey;
	json jsonheaders;
};

