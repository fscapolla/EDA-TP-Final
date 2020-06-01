#include "NodeServer.h"


/*Server constructor. Initializes io_context, acceptor and socket.
Calls waitForConnection to accept connections.*/

NodeServer::NodeServer(boost::asio::io_context& io_context_,std::string IP) :
	io_context(io_context_), acceptor(io_context_, tcp::endpoint(tcp::v4(), 80)), socket(io_context_) , nodeIP(IP) // q onda con ese puerto 80 eso q era ? // ahi creo q lucas dijo algo de remote endpoints 
{
	if (socket.is_open()) {
		socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		socket.close();
	}
	waitForConnection();
}

//Destructor. Closes open socket and acceptor.

NodeServer::~NodeServer() {
	std::cout << "\nClosing server.\n";
	if (socket.is_open()) {
		socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		socket.close();
	}

	if (acceptor.is_open())
		acceptor.close();

	std::cout << "Server is closed.\n";
}

/*Sets acceptor to accept (asynchronously).*/
void NodeServer::waitForConnection() {
	if (socket.is_open()) {
		std::cout << "Error: Can't accept new connection from an open socket" << std::endl;
		return;
	}
	if (acceptor.is_open()) {
		std::cout << "Waiting for connection.\n";
		acceptor.async_accept(socket, boost::bind(&NodeServer::connectionCallback, this, boost::asio::placeholders::error));
	}
}

//Closes socket and clears message holder.
void NodeServer::closeConnection() {
	socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	socket.close();
	int i = 0;
	while (mess[i] != NULL) {
		mess[i] = NULL;
		i++;
	}
}


/*Called when there's been a connection.*/
void NodeServer::connectionCallback(const boost::system::error_code& error) {
	if (!error) {
		//Sets socket to read request.
		socket.async_read_some
		(
			boost::asio::buffer(mess, MAXSIZE),
			boost::bind
			(
				&NodeServer::parse,
				this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}

	else
		std::cout << error.message() << std::endl;
}



/*Called when a message has been sent to client.*/
void NodeServer::messageCallback(const boost::system::error_code& error, size_t bytes_sent)
{
	if (!error)
		std::cout << "Response sent correctly.\n\n";

	else
		std::cout << "Failed to respond to connection\n\n";

	/*Once the answer was sent, it frees acceptor for a new connection.*/
	waitForConnection();
}




/*Generates http response, according to validity of input.*/
json NodeServer::generateTextResponse(void) {

	json response;

	switch (state) {
	case GET_BLOCKS:

		//aca hay q escribir el json con la resp q va
		break;
	case GET_BLOCK_HEADER:

		//aca lo mismo 
		break;
	default:
		break;
	}





	return response;
}




/*Validates input given in GET request.*/
void NodeServer::parse(const boost::system::error_code& error, size_t bytes) {
	if (!error) {

		bool isInputOk = false;

		//Creates string message from request.
		std::string message(mess);

		//Validator has the http protocol form.
		std::string validator = nodeIP + '/' + "eda_coin" + '/';

		std::string get_blocks = "get_blocks";
		std::string get_block_header = "get_block_header";
		std::string block_id = "block_id=";
		std::string count_ = "&count=";

		//If there's been a match at the beggining of the request...\

		auto it = message.find(validator);

		if (it == 0) {

			it = message.find(block_id);
			if (it != message.npos) {
				blockId = message[it + block_id.size()];
			}

			it = message.find(count_);
			if (it != message.npos) {
				count = message[it + count_.size()];
			}

		}
		else
			std::cout << "Client sent wrong input.\n";

		if (message.find(block_id)) {
			state = GET_BLOCKS;
		}
		if (message.find(count_)) {
			state = GET_BLOCK_HEADER;

		}
		answer();
	}

	//If there's been an error, prints the message.
	else
		std::cout << error.message() << std::endl;
}


/*Responds to input.*/
void NodeServer::answer() {


	/*Generates text response, according to validity of input.*/
	response = generateTextResponse();

	/*Sets socket to write (send to client).*/
	socket.async_write_some
	(
		boost::asio::buffer(response), // q onda como paso un json como respuesta http? el error q salta es q no sabe q sobrecarga usar desp preguntamos
		boost::bind
		(
			&NodeServer::messageCallback,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);

	/*Closes socket*/
	closeConnection();

}