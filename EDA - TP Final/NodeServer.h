#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include <boost\bind.hpp>
#include <chrono>
#include <fstream>
#include "json.hpp"
#include <boost/lexical_cast.hpp>
#include <boost\asio\ip\address.hpp>
#include <functional>

using boost::asio::ip::tcp;
using json = nlohmann::json;

#define MAXSIZE 1000
typedef std::function<json(std::string)> pcallback;

class NodeServer {

public:

	NodeServer(boost::asio::io_context& io_context_, std::string IP, pcallback p, unsigned int port_);
	void waitForConnection(void);
	~NodeServer();
private:


	void closeConnection(void);
	void parse(const boost::system::error_code& error, size_t bytes_sent);
	void answer();
	void generateTextResponse(void);
	void response_sent_cb(const boost::system::error_code& error, size_t bytes_sent);
	void connectionCallback(const boost::system::error_code& error);
	void messageCallback(const boost::system::error_code& error, size_t bytes_sent);
	pcallback pcback;
	std::string makeDaytimeString(int secs);
	boost::asio::io_context& io_context;
	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::ip::tcp::socket socket;

	std::string response;
	std::string nodeIP;
	char ClientInput[MAXSIZE];
	std::string blockId;
	unsigned int count;
	unsigned int state;

	json result;

	std::string msg;
	std::string date;
	std::string ServerOutput;
	int port;
	size_t FileLenght;

	std::string ClientInputStr;

};
