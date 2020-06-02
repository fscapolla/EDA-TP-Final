#pragma once
#include "Node.h"

typedef  json(*pcallback)(std::string msg);

class FullNode :
	public Node
{
public:
	FullNode();
	FullNode(unsigned int ID_, std::string IP_ ,unsigned int port_);
	~FullNode();

	json fullCallback(std::string msg);

	//Funciones para enviar mensajes.
	bool POSTBlock(unsigned int neighbourID, std::string& BlockID);
	bool POSTMerkleBlock(unsigned int neighbourID);
	bool GETBlocks(unsigned int neighbourID, std::string& blockID_, unsigned int count);

	//Funciones para generar los JSON de los mensajes
	json createJSONBlock(std::string& BlockId);
	json createJSONMerkleBlock(void);

	// funciones para responder a los GET msg

	json find_array(std::string ID_, int count);
	json find_headers(std::string ID_, int count);


	// funciones para responder a los POSTS

	json findBlockJSON(std::string message);
	json findTxJSON(std::string message);
	json findFilterJSON(std::string message);

private:
	Blockchain NodeBlockchain;
	std::vector <std::string> filters;
};

