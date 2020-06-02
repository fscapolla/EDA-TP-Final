#include "FullNode.h"



FullNode::FullNode()
{
}

FullNode::FullNode(unsigned int ID_, std::string IP_, unsigned int port_)
{
	ID = ID_;
	IP = IP_;
	port = port_;
	client = new NodeClient(IP, port +1);
	boost::asio::io_context io_context;
	server = new NodeServer(io_context, IP_, fullCallback,port);
}


FullNode::~FullNode()
{
	if (client)
		delete client;
	if (server)
		delete server;
}



/************************************************************************************************
*					                          MENSAJES											*
*																								*
*************************************************************************************************/

//POST Block
//Recibe ID del vecino, e ID del bloque a enviar
//Genera un JSON con los datos del ID del bloque (falta terminar esa funci�n) para luego adjuntarlo como header del mensaje Post
//S�lo configura el mensaje, la idea ser�a llamar a perform request (del nodo no del cliente) una vez seteado (por ah� desde el m�todo performRequest de cada nodo)
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
			//client->performRequest(); //S�lo ejecuta una vuelta de multiHandle. Para continuar us�ndolo se debe llamar a la funci�n performRequest
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
*					               GENERADORES DE JSON											*
*																								*
*************************************************************************************************/



//Genera un JSON del bloque de la blockchain que coincida con BlockId

json FullNode::createJSONBlock(std::string& BlockId)
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


//En esta fase uso lo que dice la gu�a, creo que no hay que generar el merkle path
//Cargo con datos del primer bloque del arreglo.
//Para fases futuros hay que agregar en Block.h una funci�n que recupere el MerklePath
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



/************************************************************************************************
*					               CALLBACK										*
*																								*
*************************************************************************************************/

json FullNode::fullCallback(string message) {


	json result;
	std::string ID_;
	unsigned int count_;

	result["state"] = true;


	if ((message.find("get_blocks") != std::string::npos) || (message.find("get_block_header") != std::string::npos))
	{
		unsigned int idPosition = message.find("block_id=");
		unsigned int countPosition = message.find("count=");
		string block_id("block_id=");
		string count("count=");

		if (idPosition != std::string::npos && countPosition != std::string::npos)
		{

			ID_ = message.substr(idPosition + block_id.size(), message.find_last_of("&") - idPosition - block_id.size());
			std::string tempcount = message.substr(countPosition + count.size(), message.size() - countPosition - count.size());
			count_ = std::stoi(tempcount);
		}
		else {
			result["state"] = false;
			return result;
		}

		if (message.find("get_blocks"))
		{
			result["result"] = find_array(ID_ , count_);
		}
		if (message.find("get_block_header"))
		{

			result["result"] = find_headers(ID_,count_);
		}
	}

	//Si se trata de un POSTblock guarda el block enviado
	if (message.find("send_block"))
	{
		result["result"] = findBlockJSON(message);
		if (result["result"] == "NULL") {
			result["state"] = false;
		}
	}
	//Si se trata de un POSTtransaction
	else if (message.find("send_tx"))
	{
		result["result"] = findTxJSON(message);
	}
	//Si se trata de un POSTfilter
	else if (message.find("send_filter"))
	{
		result["result"] = findFilterJSON(message);// guardar los datos
	}
	else {
		result["state"] = false;
		return result;
	}

	return result;


}



json FullNode::find_array(std::string blockID, int count) {

	auto jsonarray = json::array();
	Block block;
	int pointer;
	int numBlocks = count;
	for (int i = 0; i < NodeBlockchain.getBlocksSize(); i++)
	{
		if (NodeBlockchain.getBlocksArr()[i].getBlockID() == blockID) {
			block = NodeBlockchain.getBlocksArr()[i];
			pointer = i;
			break;
		}
	}

	if ((NodeBlockchain.getBlocksSize() - block.getHeight()) < count) {
		numBlocks = NodeBlockchain.getBlocksSize() - block.getHeight();
	}

	for (int i = 0;i < numBlocks;i++,pointer++) {
		jsonarray.push_back(createJSONBlock(NodeBlockchain.getBlocksArr()[pointer].getBlockID()));
	}
	return jsonarray;
}


json FullNode::find_headers(std::string blockID, int count) {

	auto jsonarray = json::array();
	json jsonblock;
	Block block;
	int pointer;
	int numBlocks = count;

	for (int i = 0; i < NodeBlockchain.getBlocksSize(); i++)
	{
		if (NodeBlockchain.getBlocksArr()[i].getBlockID() == blockID) {
			block = NodeBlockchain.getBlocksArr()[i];
			pointer = i;
			break;
		}
	}

	if ((NodeBlockchain.getBlocksSize() - block.getHeight()) < count) {
		numBlocks = NodeBlockchain.getBlocksSize() - block.getHeight();
	}

	for (int i = 0;i < numBlocks;i++, pointer++) {

		jsonblock["blockid"] = NodeBlockchain.getBlocksArr()[pointer].getBlockID();
		jsonblock["height"] = NodeBlockchain.getBlocksArr()[pointer].getHeight();
		jsonblock["merkleroot"] = NodeBlockchain.getBlocksArr()[pointer].getMerkleRoot();
		jsonblock["nTx"] = NodeBlockchain.getBlocksArr()[pointer].getNtx();
		jsonblock["nonce"] = NodeBlockchain.getBlocksArr()[pointer].getNonce();
		jsonblock["previousblockid"] = NodeBlockchain.getBlocksArr()[pointer].getPrevBlovkID();
		
		jsonarray.push_back(jsonblock);


	}
	return jsonarray;

}



json FullNode::findBlockJSON(std::string message) {


	

	json blockJSON = json::parse(message); // esto es si el parser hace sobrevivir solo los datros json del string 
	Block block(blockJSON);
	if (block.createMerkleTree()) {
		return blockJSON;
	}
	else {
		return "NULL";
	}

}

json FullNode::findTxJSON(std::string message) {


	json TxJSON = json::parse(message);

	return "NULL";
}


json FullNode::findFilterJSON(std::string message) {


	json filterJSON = json::parse(message);

	return "NULL";
}