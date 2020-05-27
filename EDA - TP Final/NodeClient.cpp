#include "NodeClient.h"
#include "json.hpp"

NodeClient::NodeClient(std::string IP_, int port_)
{
	IP = IP_;
	port = port_;
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
	host = "127.0.0.1:" + std::to_string(port);

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

void NodeClient::sendRequest(void)
{
	curl_multi_perform(multiHandle, &stillRunning);
}

void NodeClient::useGETmethod(std::string path_, std::string& data)
{
	method = GET;
	url = "http://" + host + path_;


	/*Prosigo a configurar CURL para usar con el método GET*/
	/*Posiblemente haya que setear más configuraciones, pero éstas van seguro*/
	if (errorCode = ERROR_FREE)
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

		//Falta setear header, creo que había que usar curlList o algo así. Después lo miro.
	}
}

void NodeClient::usePOSTmethod(std::string path_, std::string& data)
{
	method = POST;
	url = "http://" + host + path_;

	/*Prosigo a configurar CURL para usar con el método POST*/
	/*Posiblemente haya que setear más configuraciones, pero éstas van seguro*/
	/*Una vez que tengamos todas las configuraciones podemos agrupar las comunes a los dos métodos en una función aparte.*/
	if (errorCode = ERROR_FREE)
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

	}

}

void NodeClient::setIP(std::string IP_)
{
	IP = IP_;
}

void NodeClient::setPort(int port_)
{
	port = port_;
}

void NodeClient::setURL(std::string URL_)
{
	url = URL_;
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

int NodeClient::getPort(void)
{
	return port;
}

std::string NodeClient::getURL(void)
{
	return url;
}

method_n NodeClient::getMethod(void)
{
	return method;
}

int NodeClient::getRunningStatus(void)
{
	return stillRunning;
}

errorCode_n NodeClient::getErrorCode(void)
{
	return errorCode;
}

std::string NodeClient::getErrorMsg(void)
{
	return errorMsg;
}

size_t myCallback(void * contents, size_t size, size_t nmemb, void * userp)
{
	size_t realsize = size * nmemb;
	char* data = (char *)contents;
	//fprintf(stdout, "%s",data);
	std::string* s = (std::string*)userp;
	s->append(data, realsize);
	return realsize;						//recordar siempre devolver realsize
}