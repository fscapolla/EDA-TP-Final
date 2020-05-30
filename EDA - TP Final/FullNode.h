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

	//virtual bool newPOSTmessage(unsigned int neighbourID, connection_ connectionType, const json& header);
	//virtual bool newGETmessage(unsigned int neighbourID, connection_ connectionTYPE, std::string& blockId, unsigned int count);

	virtual bool POSTBlock(unsigned int neighbourID, std::string& BlockID);
	virtual bool POSTTransaction(unsigned int neighbourID, const json& header);
	virtual bool POSTMerkleBlock(unsigned int neighbourID, const json& header);
	virtual bool GETBlocks(unsigned int neighbourID, std::string& blockID_, unsigned int count);

	//Funciones para la configuración de los mensajes
	json createJSONBlock(std::string& BlockId);


	//Esta función ejecuta una vuelta de multiHandler
	virtual bool performRequest(void);

private:
	Blockchain NodeBlockchain;
};

