#pragma once
#include <iostream>
#include <string>
#include "json.hpp"
#include "C:\VCPKG\vcpkg\installed\x64-windows\include\curl\curl.h"
//#include <curl/curl.h>

typedef enum {GET, POST} method_n;
typedef enum {ERROR_FREE2, CURLINIT_ERROR, CURLPERFORM_ERROR, INVALID_DATA} errorCode_n;
size_t myCallback(void *contents, size_t size, size_t nmemb, void *userp);
using json = nlohmann::json;

class NodeClient
{
public:
	NodeClient(std::string IP_, int port_);
	NodeClient();
	~NodeClient();

	
	bool performRequest(void);
	void useGETmethod(std::string path_, const json& data);
	void usePOSTmethod(std::string path_, const json& data);

	void setIP(std::string IP_);
	void setPort(unsigned int port_);
	void setURL(std::string URL_);
	void setHost(std::string host_);
	void setMethod(method_n method_);
	void setRunningStatus(int RunningStatus);
	void setErrorCode(errorCode_n errorCode_);
	void setErrorMsg(std::string errorMsg_);
	std::string getIP(void);
	unsigned int getPort(void);
	std::string getURL(void);
	std::string getHost(void);
	method_n getMethod(void);
	int getRunningStatus(void);
	std::string getReply(void);
	errorCode_n getErrorCode(void);
	std::string getErrorMsg(void);

private:
	CURL *easyHandler, *multiHandle;
	CURLcode easyError;
	CURLMcode multiError;
	method_n method;
	std::string reply, host, url;
	std::string IP;
	unsigned int port;
	int stillRunning;
	errorCode_n errorCode;
	std::string errorMsg;
	json data_;
	json parsedReply;

};

