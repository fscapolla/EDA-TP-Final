#include "FullNode.h"



FullNode::FullNode()
{
}


FullNode::~FullNode()
{
}

bool FullNode::addNeighbour(std::string IP_, unsigned int port_)
{
	//Nodo Full puede ser vecino con cualquier otro tipo de nodo.
	if (port_ < 0)
		return false;
	else {
		neighbours[IP_] = port_;
		return true;
	}
}

bool FullNode::POSTBlock(std::string & IP_, unsigned int port_, const json& data)
{
	state = CLIENT;
	Client.setIP(IP_);
	Client.setPort(port_);
	Client.usePOSTmethod("/eda_coin/send_block", data);
	bool result=true;
	while (result)
	{
		result = Client.performRequest();
		//Acá se podría meter algo en la GUI que muestre que está cargando o algo similar
	}
	if (!result)
	{
		state = FREE;
		errorType = CLIENT_ERROR;
		errorMessage = "Could not perform BLOCK POST REQUEST";
		return result;
	}
	else
	{
		state = FREE;
		return result;
	}
}

bool FullNode::POSTTransaction(std::string & IP_, unsigned int port_, const json& data)
{
	return false;
}

bool FullNode::POSTMerkleBlock(std::string & IP_, unsigned int port_, const json& data)
{
	return false;
}

bool FullNode::POSTFilter(std::string & IP_, unsigned int port_, const json& data)
{
	return false;
}

bool FullNode::GETBlocks(std::string & IP, unsigned int port_, ::string blockID_, unsigned int count)
{
	return false;
}

bool FullNode::GETBlockHeader(std::string & IP, unsigned int port_, ::string blockID_, unsigned int count)
{
	return false;
}
