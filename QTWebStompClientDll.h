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
		url: The url of the webstomp server (example: ws://localhost/ws). The ws:// to indicate the protocol is required
		login: Self-explanatory. The username you wanna use to log in
		passcode: The password of the user. Why I didn't name it "password"? Well because in STOMP it's called passCODE.
		debug: Bool. Set to true to get qDebug messages to see what's going on.
		parent: The parent of the QObject. I almost made you believe I know what this is!
	* @author : dmaurino
	*/
	explicit QTWebStompClient(const char* url, const char* login, const char* passcode, void(*onConnectedCallback)(void), bool debug = false, QObject *parent = Q_NULLPTR);
	enum ConnectionState { Connecting, Connected, Subscribed, Closed };
	enum AckMode { Auto, Client, ClientIndividual};
	void Subscribe(const char* queueName, void(*onMessageCallback)(const StompMessage &s), AckMode ackMode = Auto);
	void Ack(const StompMessage &s);
	void Ack(const char* id);
	void Send(const StompMessage & stompMessage);
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
	ConnectionState m_connectionState;
	const char* m_login;
	const char* m_passcode;
	void(*m_onConnectedCallback)(void);
	void(*m_onMessageCallback)(const StompMessage &s);
};