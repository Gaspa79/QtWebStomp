#pragma once
#ifndef BUILD_STATIC
# if defined(QTWEBSTOMPCLIENTDLL_LIB)
#  define QTWEBSTOMPCLIENTDLL_EXPORT Q_DECL_EXPORT
# else
#  define QTWEBSTOMPCLIENTDLL_EXPORT Q_DECL_IMPORT
# endif
#else
# define QTWEBSTOMPCLIENTDLL_EXPORT
#endif

#include <QtCore/qglobal.h>
#include "StompMessage.h"
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>

class QTWEBSTOMPCLIENTDLL_EXPORT QTWebStompClient : public QObject
{

public:

	/** Description : Constructor for the WebStompClient
	* Returns : Nothing
	* Parameters :
	- url: The url of the webstomp server (example: ws://localhost/ws). The ws:// to indicate the protocol is required
	- login: Self-explanatory. The username you wanna use to log in
	- passcode: The password of the user. Why I didn't name it "password"? Well because in STOMP it's called passCODE.
	- vHost: The vHost you want to connect to. If you don't specify one, the server will try and connect you to the default one if you have access.
	- debug: Bool. Set to true to get qDebug messages to see what's going on.
	- parent: The parent of the QObject. I almost made you believe I know what this is!
	* @author : dmaurino
	*/
	explicit QTWebStompClient(const char* url, const char* login, const char* passcode, void(*onConnectedCallback)(void), const char* vHost = NULL, bool debug = false, QObject *parent = Q_NULLPTR);
	enum ConnectionState { Connecting, Connected, Subscribed, Closed };
	enum AckMode { Auto, Client, ClientIndividual};
	
	/** Description : Subscribes to a queue if you have permission to
	* Returns : Nothing
	* Parameters :
	- queueName: The name of the queue (example: /queue/MyQueue/)
	- onMessageCallback: The function that will execute when a message is received. It should have a signature of void (const StompMessage &stompMessage).
	- ackMode: How you want to ack the messages. If set to auto, the messages will be popped instantly from the queue. Other modes require you to ack messages manually.
	* @author : dmaurino
	*/
	void Subscribe(const char* queueName, void(*onMessageCallback)(const StompMessage &s), AckMode ackMode = Auto);

	/** Description : Acks a received stompMessage
	* Parameters: 
	- s: The stompmessage instance you want to ack.
	**/
	void Ack(const StompMessage &s);

	/** Description : Acks a specific id
	* Parameters:
	- id: the id of the message you want to ack.
	**/
	void Ack(const char* id);
	
	/** Description : Sends an already constructed stompmessage.
	Use the other function overload (cause it's easier) unless you want full customization of the message **/
	void Send(const StompMessage & stompMessage);

	/** Description : Sends a message to a specific queue
	* Returns : Nothing.
	* Parameters :
	- destination: A c string with the name of the queue (example: /queue/MyQueue/)
	- message: A c string containing the message (can be anything, like a json)
	- headers: (Optional) if you want to include a specific header, you can do it using this. Pass a map reference.
	* @author : dmaurino
	*/
	void Send(const char* destination, const char* message, map<std::string, std::string> &headers = map<std::string, std::string>());

Q_SIGNALS:
	void closed();

	private Q_SLOTS:
	void onConnected();
	void onTextMessageReceived(QString message);

private:
	QWebSocket m_webSocket;
	QUrl m_url;
	bool m_debug;
	bool m_SSL;
	ConnectionState m_connectionState;
	const char* m_login;
	const char* m_passcode;
	const char* m_vHost;
	void(*m_onConnectedCallback)(void);
	void(*m_onMessageCallback)(const StompMessage &s);
};