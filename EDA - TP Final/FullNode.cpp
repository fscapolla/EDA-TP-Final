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

/************************************************************************************************
*					                          MENSAJES											*
*																								*
*************************************************************************************************/

//POST Block
//Recibe ID del vecino, e ID del bloque a enviar
//Genera un JSON con los datos del ID del bloque (falta terminar esa función) para luego adjuntarlo como header del mensaje Post
//Sólo configura el mensaje, la idea sería llamar a perform request (del nodo no del cliente) una vez seteado (por ahí desde el método performRequest de cada nodo)
bool FullNode::POSTBlock(unsigned int neighbourID, std::string& blockId)
{
	if (neighbours.find(neighbourID) != neighbours.end())
	{
		if (state == FREE)
		{
			state = CLIENT;
			json block = createJSONBlock(blockId);
			client->setIP(neighbours[neighbourID].IP);
			client->setPort(neighbours[neighbourID].port);
			client->usePOSTmethod("/eda_coin/send_block", block);
			//client->performRequest(); //Sólo ejecuta una vuelta de multiHandle. Para continuar usándolo se debe llamar a la función performRequest
			return true;
		}
		else return false;
	}	
	else return false;
}

/*POST Transaction
Recibe el ID del vecino, y la transacción a enviar.
Convierte la transacción en un JSON para luego adjuntarla como header del mensaje.
Igual al caso anterior, para terminar de ejecutar llamar a performRequest del nodo (NO de client!!).*/
bool FullNode::POSTTransaction(unsigned int neighbourID, Transaction Tx_)
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

//POST Merkleblock
//Recibe el ID del vecino
//Para terminar de ejecutar usar performRequest del nodo (NO de client!!)
bool FullNode::POSTMerkleBlock(unsigned int neighbourID)
{
	if (neighbours.find(neighbourID) != neighbours.end())
	{
		if (state == FREE)
		{
			state = CLIENT;
			json jsonMerkleBlock = createJSONMerkleBlock();
			client->setIP(neighbours[neighbourID].IP);
			client->setPort(neighbours[neighbourID].port);
			client->usePOSTmethod("/eda_coin/send_merkle_block", jsonMerkleBlock);
			//client->performRequest();
			return true;
		}
		else return false;
	}
	else return false;
}

bool FullNode::GETBlocks(unsigned int neighbourID, std::string& blockID_, unsigned int count)
{
	if (neighbours.find(neighbourID) != neighbours.end())
	{
		if (state == FREE)
		{
			state = CLIENT;
			client->setIP(neighbours[neighbourID].IP);
			client->setPort(neighbours[neighbourID].port);
			client->useGETmethod("/eda_coin/get_blocks?block_id=" + blockID_ + "&count=" + to_string(count));
		}
		else return false;
	}
	else return false;
}

/************************************************************************************************
*					                         Respuestas											*
*																								*
*************************************************************************************************/

//Respuesta a los mensajes del tipo POST.
std::string FullNode::POSTreply(std::string &receivedRequest)
{
	json response;
	response["status"] = "true";
	response["result"] = NULL;
	
	//Si se trata de un POSTblock guarda el block enviado
	if (receivedRequest.find("send_block") )
	{

	}

	//Si se trata de un POSTtransaction
	else if (receivedRequest.find("send_tx"))
	{
	}

	//Si se trata de un POSTfilter
	else if (receivedRequest.find("send_filter"))
	{
	}

	/*return "HTTP/1.1 200 OK\r\nDate:" + makeDaytimeString(0) + "Location: " + "eda_coins" + "\r\nCache-Control: max-age=30\r\nExpires:" +
		makeDaytimeString(30) + "Content-Length:" + std::to_string(result.dump().length()) +
		"\r\nContent-Type: text/html; charset=iso-8859-1\r\n\r\n" + result.dump();*/
}

std::string FullNode::GETreply(std::string &receivedRequest)
{
	return receivedRequest;
}



/************************************************************************************************
*					               GENERADORES DE JSON											*
*																								*
*************************************************************************************************/

//Genera un JSON del bloque de la blockchain que coincida con BlockId
json FullNode::createJSONBlock(std::string & BlockId)
{
	json jsonblock;
	Block block;
	for (int i = 0; i < NodeBlockchain.getBlocksSize(); i++)
	{
		if (NodeBlockchain.getBlocksArr()[i].getBlockID() == BlockId) {
			block = NodeBlockchain.getBlocksArr()[i];
			break;
		}
	}
	jsonblock["blockid"] = block.getBlockID();
	jsonblock["height"] = block.getHeight();
	jsonblock["merkleroot"] = block.getMerkleRoot();
	jsonblock["nTx"] = block.getNtx();
	jsonblock["nonce"] = block.getNonce();
	jsonblock["previousblockid"] = block.getPrevBlovkID();

	auto tx = json::array();
	for (auto tx_ = 0; tx_ < block.getNtx(); tx_++)
	{
		tx += createJSONTx(block.getTxVector()[tx_]);
	}
	jsonblock["tx"] = tx;
	return jsonblock;
}

//Genera el JSON de una transacción.
json FullNode::createJSONTx(Transaction Tx_)
{
	json jsonTx;
	jsonTx["nTxin"] = Tx_.nTxin;
	jsonTx["nTxout"] = Tx_.nTxout;
	jsonTx["txid"] = Tx_.txID;

	auto vin = json::array();	//Cargo el JSON de Vin dentro del JSON de transacciones.
	for (auto vin_ = 0; vin_ < Tx_.nTxin; vin_++)
	{
		vin.push_back(json::object({ {"txid",Tx_.vIn[vin_].txID}, {"outputIndex",Tx_.vIn[vin_].outputIndex}, {"signature",Tx_.vIn[vin_].signature}, {"blockid", Tx_.vIn[vin_].LilblockID} }));
	}
	jsonTx["vin"] = vin;

	auto vout = json::array(); //Cargo el JSON de Vout dentro del JSON de transacciones.
	for (auto vout_ = 0; vout_ < Tx_.nTxout; vout_++)
	{
		vout.push_back(json::object({{ "amount",Tx_.vOut[vout_].amount },{ "publicid", Tx_.vOut[vout_].publicID}}));
	}
	jsonTx["vout"] = vout;

	return jsonTx;
}

//En esta fase uso lo que dice la guía, creo que no hay que generar el merkle path
//Cargo con datos del primer bloque del arreglo.
//Para fases futuros hay que agregar en Block.h una función que recupere el MerklePath
//Genera el JSON de un Merkle Block.
json FullNode::createJSONMerkleBlock(void)
{
	json MerkleBlock;
	json path = json::array();
	MerkleBlock["blockid"] = NodeBlockchain.getBlocksArr()[0].getBlockID();
	MerkleBlock["tx"] = createJSONTx(NodeBlockchain.getBlocksArr()[0].getTxVector()[0]);
	MerkleBlock["txPos"] = 1;
	for (int i = 0; i < NodeBlockchain.getBlocksArr()[0].getTxVector()[0].nTxin; i++)
	{
		path.push_back(json::object({ {"ID","1234"} }));
	}
	MerkleBlock["merklePath"] = path;
	return MerkleBlock;
}
