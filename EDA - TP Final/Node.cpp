#include "Node.h"



Node::Node()
{
}


Node::~Node()
{
}

bool Node::performRequest(void)
{
	if (client)
	{
		if (state == CLIENT)
		{
			if (!(client->performRequest()))
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

std::string Node::makeDaytimeString(int secs)
{
	std::chrono::system_clock::time_point time = std::chrono::system_clock::now();

	time += std::chrono::seconds(secs);

	time_t time_ = std::chrono::system_clock::to_time_t(time);
	return ctime(&time_);
}

void Node::setIP(std::string IP_)
{
	IP = IP_;
}

void Node::setPort(unsigned int port_)
{
	port = port_;
}

void Node::setID(unsigned int ID_)
{
	ID = ID_;
}

void Node::setState(state_n state_)
{
	state = state_;
}

void Node::setErrorType(errorType_n errorType_)
{
	errorType = errorType_;
}

void Node::setErrorMessage(std::string errorMessage_)
{
	errorMessage = errorMessage_;
}

std::string Node::getIP(void)
{
	return IP;
}

unsigned int Node::getPort(void)
{
	return port;
}

errorType_n Node::getErrorType(void)
{
	return errorType;
}

std::string Node::getErrorMessage(void)
{
	return errorMessage;
}

unsigned int Node::getID(void)
{
	return ID;
}

state_n Node::getState(void)
{
	return state;
}

std::map <unsigned int, Neighbour> Node::getNeighbours(void)
{
	return neighbours;
}

std::vector<std::string> Node::getFilters(void)
{
	return FilterArray;
}

bool FullNode::addNeighbour(unsigned int ID_, std::string IP_, unsigned int port_)
{
	//Nodo Full puede ser vecino con cualquier otro tipo de nodo.
	if (port_ < 0)
		return false;
	else {
		neighbours[ID_] = { IP_, port_ };
		return true;
	}
}

bool SPVNode::POSTTransaction(unsigned int neighbourID, Transaction Tx_)
{
	if (neighbours.find(neighbourID) != neighbours.end())
	{
		if (state == FREE)
		{
			state = CLIENT;
			json jsonTx = createJSONTx(Tx_);
			client->setIP(neighbours[neighbourID].IP);
			client->setPort(neighbours[neighbourID].port);
			client->usePOSTmethod("/eda_coin/send_tx", jsonTx);
			//client->performRequest();
			return true;
		}
		else return false;
	}
	else return false;
}
