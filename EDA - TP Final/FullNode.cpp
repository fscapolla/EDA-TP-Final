#include "FullNode.h"



FullNode::FullNode()
{
}

FullNode::FullNode(unsigned int ID_, std::string IP_, unsigned int port_)
{
	ID = ID_;
	IP = IP_;
	port = port_;
	client = new NodeClient(IP, port);
}


FullNode::~FullNode()
{
	if (client)
		delete client;
	if (server)
		delete server;
}

bool FullNode::addNeighbour(unsigned int ID_, std::string IP_, unsigned int port_)
{
	//Nodo Full puede ser vecino con cualquier otro tipo de nodo.
	if (port_ < 0)
		return false;
	else {
		neighbours[ID_] = { IP_, port_};
		return true;
	}
}

//POST Block campo de datos, JSON del bloque.
//Sólo configura el mensaje, la idea sería llammar perform request una vez seteado.
bool FullNode::POSTBlock(unsigned int neighbourID, std::string& blockId)
{
	state = CLIENT;
	json block = createJSONBlock(blockId);
	client->setIP(neighbours[neighbourID].IP);
	client->setPort(neighbours[neighbourID].port);
	client->usePOSTmethod("/eda_coin/send_block", block);
	client->performRequest();
	return true;
	//bool result=true;
	//while (result)
	//{
	//	result = Client.performRequest();
	//	//Acá se podría meter algo en la GUI que muestre que está cargando o algo similar
	//}
	//if (!result)
	//{
	//	state = FREE;
	//	errorType = CLIENT_ERROR;
	//	errorMessage = "Could not perform BLOCK POST REQUEST";
	//	return result;
	//}
	//else
	//{
	//	state = FREE;
	//	return result;
	//}
}

bool FullNode::POSTTransaction(unsigned int neighbourID, const json& header)
{
	state = CLIENT;

}

bool FullNode::POSTMerkleBlock(unsigned int neighbourID, const json& header)
{
	return false;
}

bool FullNode::GETBlocks(unsigned int neighbourID, std::string& blockID_, unsigned int count)
{
	return false;
}

json FullNode::createJSONBlock(std::string & BlockId)
{
	json jsonblock;
	Block block;
	for (int i = 0; i < NodeBlockchain.getBlocksSize(); i++)
	{
		if (NodeBlockchain.getBlocksArr()[i].getBlockID == BlockId) {
			block = NodeBlockchain.getBlocksArr()[i];
			break;
		}
	}
	//Acá hay que de alguna manera devolver el jJSON del bloque

}

bool FullNode::performRequest(void)
{
	if (client)
	{
		if (state == CLIENT)
		{
			if (!(client->performRequest))
			{
				state = FREE;
				return true;
			}
		}
		else
		{
			errorType = BUSY_NODE;
			errorMessage = "Node is not available to perform as client.";
			return false;
		}
	}
	else
	{
		errorType = CLIENT_ERROR;
		errorMessage = "client is not working properly....";
		return false;
	}
}
