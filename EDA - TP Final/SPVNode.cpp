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
		}
		else return false;
	}
	else return false;
}



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