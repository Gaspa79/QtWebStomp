#pragma once
#include "qtwebstompclientdll_global.h"
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include "StompMessage.cpp"

class QTWEBSTOMPCLIENTDLL_EXPORT QTWebStompClient : public QObject
{

public:
	explicit QTWebStompClient(const QUrl &url, const char* login, const char* passcode, void(*onConnectedCallback)(void), bool debug = false, QObject *parent = Q_NULLPTR);
	enum ConnectionState { Connecting, Connected, Subscribed, Closed };
	enum AckMode { Auto, Client, ClientIndividual};
	void Subscribe(const char* queueName, void(*onMessageCallback)(const StompMessage &s), AckMode ackMode = Auto);
	void Ack(const StompMessage &s);
	void Ack(const char* id);

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