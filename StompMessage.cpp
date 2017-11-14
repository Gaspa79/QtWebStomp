#include <map>
#include <string>
#include <vector>

using namespace std;

class StompMessage {
public:
	/// This creates a stompmessage from a message received string
	StompMessage(const char* rawMessage){

		string strMessage(rawMessage);
		vector<string> messageVector = messageToVector(strMessage, "\n");
		m_message = messageVector.back(); // deep copies
		messageVector.pop_back();
		bool first = true;
		for (const auto& header : messageVector)
		{
			size_t pos = header.find_first_of(':', 0);
			string key = header.substr(0, pos);
			if (first)
			{
				m_messageType = key;
				first = false;
				continue;
			}
			string value = header.substr(++pos, value.length() - pos);
			m_headers[key] = value;
		}
	}

	StompMessage(string messageType, map<string, string> headers, const char* messageBody = "")
	{
		m_messageType = std::string(messageType);
		m_message = messageBody;
		m_headers = headers;
	}

	std::string toString() const
	{
		std::string result = m_messageType + "\u000A";
		for (const auto &header : m_headers)
		{
			result += header.first + ":" + header.second + "\u000A";
		}

		result += "\u000A" + m_message + "\u0000";

		return result;
	}

	enum MessageType { CONNECT, SUBSCRIBE, SEND, MESSAGE };
	
	map<string, string> m_headers;
	string m_message;
	string m_messageType;

private:

	vector<string> messageToVector(const string& str, const string& delim)
	{
		vector<string> messageParts;
		size_t prev = 0, pos = 0;
		bool last = false;
		do
		{
			if (last)
			{
				auto message = str.substr(prev, str.length() - prev);
				messageParts.push_back(message);
				break;
			}

			pos = str.find(delim, prev);
			if (pos == string::npos) pos = str.length();
			string token = str.substr(prev, pos - prev);
			if (!token.empty())
			{
				messageParts.push_back(token);
			}
			else
			{
				// If the token is empty the headers finished =) Just the body left!
				last = true;
			}
			prev = pos + delim.length();
		} while (pos < str.length() && prev <= str.length()); // I know but we have to do it at least once, so...
		return messageParts;
	}
};


