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

//POST Block
//Recibe ID del vecino, e ID del bloque a enviar
//Genera un JSON con los datos del ID del bloque (falta terminar esa funci�n) para luego adjuntarlo como header del mensaje Post
//S�lo configura el mensaje, la idea ser�a llammar perform request (del nodo no de client) una vez seteado (por ah� desde el m�todo performRequest de cada nodo)
bool FullNode::POSTBlock(unsigned int neighbourID, std::string& blockId)
{
	state = CLIENT;
	json block = createJSONBlock(blockId);
	client->setIP(neighbours[neighbourID].IP);
	client->setPort(neighbours[neighbourID].port);
	client->usePOSTmethod("/eda_coin/send_block", block);
	client->performRequest(); //S�lo ejecuta una vuelta de multiHandle. Para continuar us�ndolo se debe llamar a la funci�n performRequest
	return true;
}

/*POST Transaction
Recibe el ID del vecino, y la transacci�n a enviar.
Convierte la transacci�n en un JSON para luego adjuntarla como header del mensaje.
Igual al caso anterior, para terminar de ejecutar llamar a performRequest del nodo (NO de client!!).*/
bool FullNode::POSTTransaction(unsigned int neighbourID, Transaction Tx_)
{
	state = CLIENT;
	json jsonTx = createJSONTx(Tx_);
	client->setIP(neighbours[neighbourID].IP);
	client->setPort(neighbours[neighbourID].port);
	client->usePOSTmethod("/eda_coin/send_tx", jsonTx);
	client->performRequest();
	return true;
}

//POST Merkleblock
//Recibe el ID del vecino
//Para terminar de ejecutar usar performRequest del nodo (NO de client!!)
bool FullNode::POSTMerkleBlock(unsigned int neighbourID)
{
	state = CLIENT;
	json jsonMerkleBlock = createJSONMerkleBlock();
	client->setIP(neighbours[neighbourID].IP);
	client->setPort(neighbours[neighbourID].port);
	client->usePOSTmethod("/eda_coin/send_merkle_block", jsonMerkleBlock);
	client->performRequest();
	return true;
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
	jsonblock["blockid"] = block.getBlockID();
	jsonblock["height"] = block.getHeight();
	jsonblock["merkleroot"] = block.getMerkleRoot();
	jsonblock["nTx"] = block.getNtx();
	jsonblock["nonce"] = block.getNonce();
	jsonblock["previousblockid"] = block.getPrevBlovkID();

	auto tx = json::array();
	for (auto tx_ = 0; tx_ < block.getNtx(); tx_++)
	{
		tx += json::parse(createJSONTx(block.getTxVector()[tx_]));
	}
	jsonblock["tx"] = tx;
	return jsonblock;
}

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

//En esta fase uso lo que dice la gu�a, creo que no hay que generar el merkle path
//Cargo con datos del primer bloque del arreglo.
//Para fases futuros hay que agregar en Block.h una funci�n que recupere el MerklePath
json FullNode::createJSONMerkleBlock(void)
{
	json MerkleBlock;
	json path = json::array();
	MerkleBlock["blockid"] = NodeBlockchain.getBlocksArr()[0].getBlockID();
	MerkleBlock["tx"] = json::parse(createJSONTx(NodeBlockchain.getBlocksArr()[0].getTxVector()[0]));
	MerkleBlock["txPos"] = 1;
	for (int i = 0; i < NodeBlockchain.getBlocksArr()[0].getTxVector()[0].nTxin; i++)
	{
		path.push_back(json::object({ {"ID","1234"} }));
	}
	MerkleBlock["merklePath"] = path;
	return MerkleBlock;
}