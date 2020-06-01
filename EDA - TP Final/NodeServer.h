#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include <boost\bind.hpp>
#include <chrono>
#include <fstream>
#include "json.hpp"

using boost::asio::ip::tcp;
using json = nlohmann::json;

enum { GET_BLOCKS, GET_BLOCK_HEADER };

#define MAXSIZE 1000

class NodeServer
{
public:
	NodeServer(boost::asio::io_context& io_context_, std::string IP);

	~NodeServer();
private:
	void waitForConnection(void);
	void closeConnection(void);

	void parse(const boost::system::error_code& error, size_t bytes);


	void answer();
	json generateTextResponse(void);

	void connectionCallback(const boost::system::error_code& error);
	void messageCallback(const boost::system::error_code& error, size_t bytes_sent);

	boost::asio::io_context& io_context;
	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::ip::tcp::socket socket;


	char mess[MAXSIZE];
	json response;
	std::string nodeIP;


	unsigned int blockId;
	unsigned int count;
	unsigned int state;
};
