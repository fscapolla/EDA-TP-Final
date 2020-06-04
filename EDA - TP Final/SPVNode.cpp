#include "SPVNode.h"



SPVNode::SPVNode()
{
}

SPVNode::SPVNode(unsigned int ID_, std::string IP_, unsigned int port_)
{
	ID = ID_;
	IP = IP_;
	port = port_;
	client = new NodeClient(IP, port);
	server = new NodeServer(io_context,IP,boost::bind(&SPVNode::SpvCallback,this,_1),port);
}


SPVNode::~SPVNode()
{
	delete client;
}

std::string SPVNode::getKey(void)
{
	return publickey;
}

void SPVNode::setKey(std::string Key_)
{
	publickey = Key_;
}


/************************************************************************************************
*					                          MENSAJES											*
*																								*
*************************************************************************************************/

bool SPVNode::POSTFilter(unsigned int neighbourID, std::string key)
{
	if (neighbours.find(neighbourID) != neighbours.end())
	{
		if (state == FREE)
		{
			state = CLIENT;
			json jsonFilter = createJSONFilter(key);
			client->setIP(neighbours[neighbourID].IP);
			client->setPort(neighbours[neighbourID].port);
			client->usePOSTmethod("/eda_coin/send_filter", jsonFilter);
			return true;
		}
		else return false;
	}
	else return false;
}


bool SPVNode::GETBlockHeader(unsigned int neighbourID, std::string & blockID_, unsigned int count) //Falta terminar
{
	if (neighbours.find(neighbourID) != neighbours.end())
	{
		if (state == FREE)
		{
			state = CLIENT;
			client->setIP(neighbours[neighbourID].IP);
			client->setPort(neighbours[neighbourID].port);
			client->useGETmethod("/eda_coin/get_block_header?block_id=" + blockID_ + "&count=" + to_string(count));
			return true;
		}
		else return false;
	}
	else return false;
}

bool SPVNode::makeTransaction(unsigned int neighbourID, std::string & wallet, unsigned int amount)
{
	if (neighbours.find(neighbourID) != neighbours.end())
	{
		if (state == FREE)
		{
			json jsonTx;

			jsonTx["nTxin"] = 0;
			jsonTx["nTxout"] = 1;
			jsonTx["txid"] = "7E46A3BC";
			jsonTx["vin"] = json();
			json vout_;
			vout_["amount"] = amount;
			vout_["publicid"] = wallet;
			jsonTx["vout"] = vout_;

			state = CLIENT;
			client->setIP(neighbours[neighbourID].IP);
			client->setPort(neighbours[neighbourID].port);
			client->usePOSTmethod("/eda_coin/send_tx", jsonTx);
			return true;
		}
		else {
			errorType = BUSY_NODE;
			errorMessage = "Node is not available to perform as client";
			return false;
		}
	}
	else {
		errorType = NOT_NEIGHBOUR;
		errorMessage = "Requested server is not a Neighbour of current Node";
		return false;
	}
}


/************************************************************************************************
*					               GENERADORES DE JSON											*
*																								*
*************************************************************************************************/



json SPVNode::createJSONFilter(std::string Id_)
{
	json jsonFilter;
	jsonFilter["id"] = Id_;
	return jsonFilter;
}




/************************************************************************************************
*					               CALLBACK											*
*																								*
*************************************************************************************************/


json SPVNode::SpvCallback(string message) {


	json result;

	result["state"] = true;

	//Si se trata de un POSTmerkletree
	if (message.find("send_merkle_block"))
	{
		result["result"] = findMBlockJSON(message);// guardar los datos
	}
	else {
		result["state"] == false;
	}

	return result;


}



json SPVNode::findMBlockJSON(std::string message) {


	json mBlockJSON = json::parse(message);

	return "NULL";
}