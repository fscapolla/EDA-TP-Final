#include "NodeClient.h"


NodeClient::NodeClient(std::string IP_, int port_)
{
	IP = IP_;
	own_port = port_;
	stillRunning = 1;
	easyHandler = curl_easy_init();
	if (!easyHandler)
	{
		this->setErrorCode(CURLINIT_ERROR);
		this->setErrorMsg("Unable to start curl");
	}
	multiHandle = curl_multi_init();
	if (!multiHandle)
	{
		this->setErrorCode(CURLINIT_ERROR);
		this->setErrorMsg("Unable to start curl");
	}
}

NodeClient::NodeClient()
{
}


NodeClient::~NodeClient()
{
	curl_easy_cleanup(easyHandler);
	curl_multi_cleanup(multiHandle);

	curl_multi_remove_handle(multiHandle, easyHandler);
}

bool NodeClient::performRequest(void)
{
	if (IP.length() && port)
	{
		/*static bool isFinished = false;*/

		bool res = true;
		if (stillRunning)
		{
			multiError = curl_multi_perform(multiHandle, &stillRunning);
			if (multiError != CURLE_OK)
			{
				errorCode = CURLPERFORM_ERROR;
				errorMsg = "Could not perform curl.";
			}

		}
		else
		{
			//Se limpia curl
			curl_easy_cleanup(easyHandler);
			curl_multi_cleanup(multiHandle);
			stillRunning = 1;
			parsedReply = json::parse(reply);
			res = false;
		}

		return res;
	}
	else
	{
		errorCode = INVALID_DATA;
		errorMsg = "Invalid data.";
		return false;
	}
}

void NodeClient::useGETmethod(std::string path_)
{
	method = GET;
	host = IP + ":" + std::to_string(port);
	url = "http://" + host + path_;
	//struct curl_slist* list = nullptr;

	/*Prosigo a configurar CURL para usar con el método GET*/
	if (errorCode == ERROR_FREE2)
	{
		//Se configura la URL de la página
		curl_easy_setopt(easyHandler, CURLOPT_URL, url.c_str());
		//Se configura para trabajar con HTTP
		curl_easy_setopt(easyHandler, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
		//Configuramos para que curl use myCallback al momento de escribir y a reply como userData.
		curl_easy_setopt(easyHandler, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(easyHandler, CURLOPT_WRITEDATA, &reply);
		//Configuramos para recibir info de error, útil para debuguear
		curl_easy_setopt(easyHandler, CURLOPT_VERBOSE, 1L);
		//Configuro para que curl pueda seguir redireccionamiento de ser necesario
		curl_easy_setopt(easyHandler, CURLOPT_FOLLOWLOCATION, 1L);
		//Configuro el puerto de destino
		curl_easy_setopt(easyHandler, CURLOPT_PORT, port);
		//Configuro el propio puerto.
		curl_easy_setopt(easyHandler, CURLOPT_LOCALPORT, own_port);
		//Set handler y multiHandle
		curl_multi_add_handle(multiHandle, easyHandler);
		//Configuro el header
		/*list=curl_slist_append(list, data.dump().c_str());
		curl_easy_setopt(easyHandler, CURLOPT_HTTPHEADER, list);*/
	}
}

void NodeClient::usePOSTmethod(std::string path_, const json& data)
{
	method = POST;
	host = IP + ":" + std::to_string(port);
	url = "http://" + host + path_;
	struct curl_slist* list = nullptr;
	reply.clear();
	myjson = data.dump();

	std::string line("Content-Type: application/json;charset=UTF-8");

	/*Prosigo a configurar CURL para usar con el método POST*/
	if (errorCode == ERROR_FREE2)
	{
		list = curl_slist_append(list, line.c_str());
		curl_easy_setopt(easyHandler, CURLOPT_HTTPHEADER, list);
		curl_easy_setopt(easyHandler, CURLOPT_POSTFIELDS, myjson.c_str());
		curl_easy_setopt(easyHandler, CURLOPT_POSTFIELDSIZE, -1);
		curl_easy_setopt(easyHandler, CURLOPT_POST, 1);
		//Se configura la URL de la página
		curl_easy_setopt(easyHandler, CURLOPT_URL, url.c_str());
		//Configuro el propio puerto
		curl_easy_setopt(easyHandler, CURLOPT_LOCALPORT, own_port);
		//Configuro el puerto dedestino
		curl_easy_setopt(easyHandler, CURLOPT_PORT, port);
		//Se configura para trabajar con HTTP
		curl_easy_setopt(easyHandler, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
		//Configuramos para que curl use myCallback al momento de escribir y a reply como userData.
		curl_easy_setopt(easyHandler, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(easyHandler, CURLOPT_WRITEDATA, &reply);
		//Configuramos para recibir info de error, útil para debuguear
		curl_easy_setopt(easyHandler, CURLOPT_VERBOSE, 1L);
		//Configuro para que curl pueda seguir redireccionamiento de ser necesario
		curl_easy_setopt(easyHandler, CURLOPT_FOLLOWLOCATION, 1L);
		//Set handler y multiHandle
		curl_multi_add_handle(multiHandle, easyHandler);
		//Configuro el header



		//Cofiguro el header

	}
}

void NodeClient::setIP(std::string IP_)
{
	IP = IP_;
}

void NodeClient::setPort(unsigned int port_)
{
	port = port_;
}

void NodeClient::setURL(std::string URL_)
{
	url = URL_;
}

void NodeClient::setHost(std::string host_)
{
	host = host_;
}

void NodeClient::setMethod(method_n method_)
{
	method = method_;
}

void NodeClient::setRunningStatus(int RunningStatus)
{
	stillRunning = RunningStatus;
}

void NodeClient::setErrorCode(errorCode_n errorCode_)
{
	errorCode = errorCode_;
}

void NodeClient::setErrorMsg(std::string errorMsg_)
{
	errorMsg = errorMsg_;
}

std::string NodeClient::getIP(void)
{
	return IP;
}

unsigned int NodeClient::getPort(void)
{
	return port;
}

std::string NodeClient::getURL(void)
{
	return url;
}

std::string NodeClient::getHost(void)
{
	return host;
}

method_n NodeClient::getMethod(void)
{
	return method;
}

int NodeClient::getRunningStatus(void)
{
	return stillRunning;
}

std::string NodeClient::getReply(void)
{
	return reply;
}

errorCode_n NodeClient::getErrorCode(void)
{
	return errorCode;
}

std::string NodeClient::getErrorMsg(void)
{
	return errorMsg;
}

size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	char* data = (char*)contents;
	//fprintf(stdout, "%s",data);
	std::string* s = (std::string*)userp;
	s->append(data, realsize);
	return realsize;						//recordar siempre devolver realsize
}