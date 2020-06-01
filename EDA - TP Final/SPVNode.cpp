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
}


SPVNode::~SPVNode()
{
	delete client;
}

bool SPVNode::addNeighbour(unsigned int neighbourID, std::string IP_, unsigned int port_)
{
	if (port_ < 0)
		return false;
	else {
		neighbours[neighbourID] = { IP_, port_ };
		return true;
	}
}

/************************************************************************************************
*					                          MENSAJES											*
*																								*
*************************************************************************************************/

bool SPVNode::POSTFilter(unsigned int neighbourID)
{
	if (neighbours.find(neighbourID) != neighbours.end())
	{
		if (state == FREE)
		{
			state = CLIENT;
			json jsonFilter = createJSONFilter(to_string(ID));
			client->setIP(neighbours[neighbourID].IP);
			client->setPort(neighbours[neighbourID].port);
			client->usePOSTmethod("/eda_coin/send_filter", jsonFilter);
			return true;
		}
		else return false;
	}
	else return false;
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

bool SPVNode::GETBlockHeader(unsigned int neighbourID, std::string & blockID_, unsigned int count)
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
		else return false;
	}
	else return false;
}

std::string SPVNode::POSTreply(std::string & receivedRequest, unsigned int clientPort) //Falta terminar
{
	json response;
	response["status"] = false;
	/*for (auto& neighbour : neighbours) 
	{
		if (neighbour.second.port + 1 == clientPort)
			receivedMessage = neighbour.first;
	}*/

	return "HTTP/1.1 200 OK\r\nDate:" + makeDaytimeString(0) + "Location: " + "eda_coins" + "\r\nCache-Control: max-age=30\r\nExpires:" +
		makeDaytimeString(30) + "Content-Length:" + std::to_string(response.dump().length()) +
		"\r\nContent-Type: " + "text/html" + "; charset=iso-8859-1\r\n\r\n" + response.dump();
}

std::string SPVNode::GETreply(std::string & receivedRequest, unsigned int clientPort) //Falta terminar
{
	/*for (auto& neighbour : neighbours)
	{
	if (neighbour.second.port + 1 == clientPort)
	receivedMessage = neighbour.first;
	}*/

	json response;
	response["status"] = true;
	response["result"] = NULL;

	//Si se recibió un request de envío de bloque
	if (receivedRequest.find("send_merkle_block") != std::string::npos)
	{

	}
	else
	{
		//Error de contenido
		response["status"] = false;
		response["result"] = 2;
	}

	/*return "HTTP/1.1 200 OK\r\nDate:" + makeDaytimeString(0) + "Location: " + "eda_coins" + "\r\nCache-Control: max-age=30\r\nExpires:" +
		makeDaytimeString(30) + "Content-Length:" + std::to_string(response.dump().length()) +
		"\r\nContent-Type: " + "text/html" + "; charset=iso-8859-1\r\n\r\n" + response.dump();*/
}

/************************************************************************************************
*					                         Respuestas											*
*																								*
*************************************************************************************************/


/************************************************************************************************
*					               GENERADORES DE JSON											*
*																								*
*************************************************************************************************/

json SPVNode::createJSONTx(Transaction Tx_)
{
	json jsonTx;
	jsonTx["nTxin"] = Tx_.nTxin;
	jsonTx["nTxout"] = Tx_.nTxout;
	jsonTx["txid"] = Tx_.txID;

	auto vin = json::array();	//Cargo el JSON de Vin dentro del JSON de transacciones.
	for (auto vin_ = 0; vin_ < Tx_.nTxin; vin_++)
	{
		vin.push_back(json::object({ { "txid",Tx_.vIn[vin_].txID },{ "outputIndex",Tx_.vIn[vin_].outputIndex },{ "signature",Tx_.vIn[vin_].signature },{ "blockid", Tx_.vIn[vin_].LilblockID } }));
	}
	jsonTx["vin"] = vin;

	auto vout = json::array(); //Cargo el JSON de Vout dentro del JSON de transacciones.
	for (auto vout_ = 0; vout_ < Tx_.nTxout; vout_++)
	{
		vout.push_back(json::object({ { "amount",Tx_.vOut[vout_].amount },{ "publicid", Tx_.vOut[vout_].publicID } }));
	}
	jsonTx["vout"] = vout;

	return jsonTx;
}

json SPVNode::createJSONFilter(std::string Id_)
{
	json jsonFilter;
	jsonFilter["id"] = Id_;
	return jsonFilter;
}