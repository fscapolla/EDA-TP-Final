#pragma once
#include "Node.h"


class FullNode :
	public Node
{
public:
	FullNode();
	~FullNode();
	virtual bool addNeighbour(std::string IP_, unsigned int port_);

	virtual bool POSTBlock(std::string& IP_, unsigned int port_, const json& data);
	virtual bool POSTTransaction(std::string& IP_, unsigned int port_, const json& data);
	virtual bool POSTMerkleBlock(std::string& IP_, unsigned int port_, const json& data);
	virtual bool POSTFilter(std::string& IP_, unsigned int port_, const json& data);
	virtual bool GETBlocks(std::string& IP, unsigned int port_, ::string blockID_, unsigned int count);
	virtual bool GETBlockHeader(std::string& IP, unsigned int port_, ::string blockID_, unsigned int count);

private:
	Blockchain NodeBlockchain;
};

