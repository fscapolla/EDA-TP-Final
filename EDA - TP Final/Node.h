#pragma once
#include "Blockchain.h"
#include "NodeClient.h"
#include "NodeServer.h"
#include <string>

typedef enum {FREE,CLIENT,SERVER} state_n;
typedef enum {ERROR_FREE, CLIENT_ERROR, SERVER_ERROr} errorType_n;

//struct Neighbour {
//	Neighbour() {}
//	Neighbour(const std::string& IP_, unsigned int port_)
//	{
//		IP = IP_;
//		port = port_;
//	}
//	
//	//Neighbour& operator=(const Neighbour& neighbour) { IP = neighbour.IP; port = neighbour.port; return *this; }
//
//	std::string IP;
//	unsigned int port;
//};

struct MerkleBlock {
	std::string BlockId;
	vector<Transaction> TxVector_;
	unsigned int TxPosition;
	vector<std::string> merklePath;
};

class Node
{
public:
	Node();
	~Node();
	//Función para agregar vecinos
	virtual bool addNeighbour(std::string& IP_, unsigned int port_)=0;
	
	//Funciones para enviar mensajes.
	virtual bool POSTBlock(std::string& IP_, unsigned int port_, const json& data)=0;
	virtual bool POSTTransaction(std::string& IP_, unsigned int port_, const json& data)=0;
	virtual bool POSTMerkleBlock(std::string& IP_, unsigned int port_, const json& data)=0;
	virtual bool POSTFilter(std::string& IP_, unsigned int port_, const json& data)=0;
	virtual bool GETBlocks(std::string& IP, unsigned int port_,::string blockID_, unsigned int count) = 0;
	virtual bool GETBlockHeader(std::string& IP, unsigned int port_, ::string blockID_, unsigned int count) = 0;

	void setIP(std::string IP_);
	void setPort(unsigned int port_);
	void setErrorType(errorType_n errorType_);
	void setErrorMessage(std::string errorMessage_);
	std::string getIP(void);
	unsigned int getPort(void);
	errorType_n getErrorType(void);
	std::string getErrorMessage(void);
	std::map <std::string, unsigned int> getNeighbours(void);
	std::vector <std::string> getFilters(void);


protected:
	std::string IP;
	unsigned int port;
	state_n state;
	NodeClient Client;
	NodeServer Server;
	errorType_n errorType;
	std::string errorMessage;
	/*std::vector <Neighbour> neighbours;*/
	std::map <std::string, unsigned int> neighbours;
	std::vector <std::string> FilterArray;
	//std::vector <Transaction> TxVector;
	//std::vector <MerkleBlock> MerkleBlockVector;

};

