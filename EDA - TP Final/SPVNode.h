#pragma once
#include "Node.h"
class SPVNode :
	public Node
{
public:
	SPVNode();
	~SPVNode();

	virtual bool addNeighbour(std::string IP_, unsigned int port_);

private:
};

