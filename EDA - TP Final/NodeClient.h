#pragma once
#include <iostream>
#include <string>
#include <curl/curl.h>

typedef enum {GET, POST} method_n;
typedef enum {ERROR_FREE, CURLINIT_ERROR} errorCode_n;
size_t myCallback(void *contents, size_t size, size_t nmemb, void *userp);

class NodeClient
{
public:
	NodeClient(std::string IP_, int port_);
	NodeClient();
	~NodeClient();

	
	void sendRequest(void);
	void useGETmethod(std::string path_, std::string& data);
	void usePOSTmethod(std::string path_, std::string& data);

	void setIP(std::string IP_);
	void setPort(int port_);
	void setURL(std::string URL_);
	void setMethod(method_n method_);
	void setRunningStatus(int RunningStatus);
	void setErrorCode(errorCode_n errorCode_);
	void setErrorMsg(std::string errorMsg_);
	std::string getIP(void);
	int getPort(void);
	std::string getURL(void);
	method_n getMethod(void);
	int getRunningStatus(void);
	errorCode_n getErrorCode(void);
	std::string getErrorMsg(void);

private:
	CURL *easyHandler, *multiHandle;
	CURLcode easyError;
	CURLMcode multiError;
	method_n method;
	std::string reply, host, url;
	std::string IP;
	int port;
	int stillRunning;
	errorCode_n errorCode;
	std::string errorMsg;


};

