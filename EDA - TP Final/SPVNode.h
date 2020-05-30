#pragma once
#include "Node.h"


class SPVNode :
	public Node
{
public:
	SPVNode();
	SPVNode(unsigned int ID_, std::string IP_, unsigned int port_);
	~SPVNode();

	virtual bool addNeighbour(unsigned int neighbourID, std::string IP_, unsigned int port_);

	virtual bool POSTFilter(unsigned int neighbourID, const json& header);
	virtual bool POSTTransaction(unsigned int neighbourID, const json& header);
	virtual bool GETBlockHeader(unsigned int neighbourID, std::string& blockID_, unsigned int count);

private:
};

