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
	state = CLIENT;
	json jsonFilter = createJSONFilter(to_string(ID));
	client->setIP(neighbours[neighbourID].IP);
	client->setPort(neighbours[neighbourID].port);
	client->usePOSTmethod("/eda_coin/send_filter", jsonFilter);
	return true;
}

bool SPVNode::POSTTransaction(unsigned int neighbourID, Transaction Tx_)
{
	state = CLIENT;
	json jsonTx = createJSONTx(Tx_);
	client->setIP(neighbours[neighbourID].IP);
	client->setPort(neighbours[neighbourID].port);
	client->usePOSTmethod("/eda_coin/send_tx", jsonTx);
	//client->performRequest();
	return true;
}

bool SPVNode::GETBlockHeader(unsigned int neighbourID, std::string & blockID_, unsigned int count) //Falta terminar
{
	state = CLIENT;
	json data;
	client->setIP(neighbours[neighbourID].IP);
	client->setPort(neighbours[neighbourID].port);
	client->useGETmethod("/eda_coin/get_block_header?block_id="+blockID_+"&count="+to_string(count),data);
	return true;
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