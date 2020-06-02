#pragma once
#include "Blockchain.h"
#include "NodeClient.h"
#include "NodeServer.h"
#include <chrono>
#include <map>

typedef enum {FREE,CLIENT,SERVER} state_n;
typedef enum {ERROR_FREE, CLIENT_ERROR, SERVER_ERROR, BUSY_NODE} errorType_n;
typedef enum {POSTBLOCK, POSTTRANSACTION, POSTMERKLE, POSTFILTER, GETBLOCKS, GETHEADER} connection_;

struct Neighbour {
	std::string IP;
	unsigned int port;
};

//struct MerkleBlock {
//	std::string BlockId;
//	vector<Transaction> TxVector_;
//	unsigned int TxPosition;
//	vector<std::string> merklePath;
//};

class Node
{
public:
	Node();
	~Node();
	//Función para agregar vecinos
	bool addNeighbour(unsigned int ID_,std::string& IP_, unsigned int port_);
	
	//Funciones para enviar mensajes.
	virtual bool POSTTransaction(unsigned int neighbourID, Transaction Tx_)=0;

	virtual std::string makeDaytimeString(int secs);

	void setIP(std::string IP_);
	void setPort(unsigned int port_);
	void setID(unsigned int ID_);
	void setState(state_n state_);
	void setErrorType(errorType_n errorType_);
	void setErrorMessage(std::string errorMessage_);
	std::string getIP(void);
	unsigned int getPort(void);
	unsigned int getID(void);
	state_n getState(void);
	errorType_n getErrorType(void);
	std::string getErrorMessage(void);
	std::map <unsigned int, Neighbour> getNeighbours(void);
	std::vector <std::string> getFilters(void);


protected:

	void TxCallback(string respuesta);
	std::string IP;
	unsigned int port;
	unsigned int ID;
	int sentMessage;
	int receivedMessage;
	state_n state;
	NodeClient *client;
	NodeServer *server;
	errorType_n errorType;
	std::string errorMessage;
	/*std::vector <Neighbour> neighbours;*/
	std::map <unsigned int, Neighbour> neighbours;
	std::vector <std::string> FilterArray;
	//std::vector <Transaction> TxVector;
	//std::vector <MerkleBlock> MerkleBlockVector;

};

