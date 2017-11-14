#pragma once
#include "qtwebstompclientdll_global.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

class QTWEBSTOMPCLIENTDLL_EXPORT StompMessage {
public:
	/// This creates a stompmessage from a message received string
	StompMessage(const char* rawMessage);

	StompMessage(string messageType, map<string, string> headers, const char* messageBody = "");
	
	std::string toString() const;

	enum MessageType { CONNECT, SUBSCRIBE, SEND, MESSAGE };
	
	map<string, string> m_headers;
	string m_message;
	string m_messageType;

private:

	vector<string> messageToVector(const string& str, const string& delim);
};


