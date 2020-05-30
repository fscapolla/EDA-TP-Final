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

bool SPVNode::POSTFilter(unsigned int neighbourID, const json & header)
{
	return false;
}

bool SPVNode::POSTTransaction(unsigned int neighbourID, const json & header)
{
	return false;
}

bool SPVNode::GETBlockHeader(unsigned int neighbourID, std::string & blockID_, unsigned int count)
{
	return false;
}
