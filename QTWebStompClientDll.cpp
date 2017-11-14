#pragma once
#include "QTWebStompClientDll.h"




QTWebStompClient::QTWebStompClient(const QUrl &url, const char* login, const char* passcode, void(*onConnected)(void), bool debug, QObject *parent)
{
	m_debug = debug;
	m_login = login;
	m_passcode = passcode;
	m_onConnectedCallback = onConnected;
	if (m_debug) {
		qDebug() << "Connecting to WebSocket server:" << url;
	}
	connect(&m_webSocket, &QWebSocket::connected, this, &QTWebStompClient::onConnected);
	connect(&m_webSocket, &QWebSocket::disconnected, this, &QTWebStompClient::closed);
	m_webSocket.open(QUrl(url));
}


void QTWebStompClient::onConnected()
{
	if (m_debug) {
		qDebug() << "-----------------------" << endl << "Connected to Websocket!" << endl << "-----------------------" << endl;
	}
	connect(&m_webSocket, &QWebSocket::textMessageReceived,
		this, &QTWebStompClient::onTextMessageReceived);

	m_connectionState = Connecting;
	QString subscribeFrame = "CONNECT\u000Aaccept-version:1.2\u000Alogin:{Login}\u000Apasscode:{Passcode}\u000A\u000A\u0000";
	subscribeFrame.replace("{Login}", m_login);
	subscribeFrame.replace("{Passcode}", m_passcode);
	QString subscribeFrameMessage = QString(subscribeFrame.data(), subscribeFrame.size() + 1); // solves the null-terminator issue
	m_webSocket.sendTextMessage(subscribeFrameMessage);

	qDebug() << "Sent message" << subscribeFrameMessage;
}

void QTWebStompClient::onTextMessageReceived(QString message)
{
	StompMessage stompMessage(message.toStdString().c_str());
	switch (m_connectionState) {

	case Connecting:
		qDebug() << "Connection response: " << stompMessage.toString().c_str();
		if (stompMessage.m_messageType == "CONNECTED")
		{
			if (m_debug)
			{
				qDebug() << "--------------------" << endl << "Connected to STOMP!" << endl << "--------------------" << endl;
			}
			m_connectionState = Connected;
			if (this->m_onConnectedCallback == NULL)
			{
				qDebug() << "WARNING: No callback selected for connection";
			}
			else
			{
				this->m_onConnectedCallback();
			}
		}
		else
		{
			if (m_debug)
			{
				qDebug() << "Message type CONNECTED expected, got " << stompMessage.m_messageType.c_str();
			}
			// TODO: Throw exception
		}
		break;

	case Subscribed:
		// TODO: Improve check, maybe different messages are allowed when subscribed
		if (stompMessage.m_messageType == "MESSAGE")
		{
			if (m_debug)
			{

				qDebug() << "Message received from queue!" << endl << stompMessage.toString().c_str();
				// decode the message and print it on the screen, later this will just return it.
				m_onMessageCallback(stompMessage);
			}
		}
		else
		{
			if (m_debug)
			{
				qDebug() << "Message type MESSAGE expected, got " << stompMessage.toString().c_str();
			}
			// TODO: Throw exception
		}
		break;

	default:
		break;
	}

}

// TODO: Change to ack client-individual
// TODO: Change to have multiple ids (so we can handle more than one subscription)
void QTWebStompClient::Subscribe(const char* queueName, void(*onMessageCallback)(const StompMessage &s), QTWebStompClient::AckMode ackMode)
{
	if (m_connectionState != Connected)
	{
		//TODO: excception
		qDebug() << "Subscribe without connection?";
		throw new exception("Cannot subscribe when connection hasn't finished. Try using the callback function for onConnect to subscribe");
	}
	map<string, string> headers;
	headers["id"] = "0";
	headers["destination"] = std::string(queueName);
	switch (ackMode) {
		case Client:
			headers["ack"] = "client";
			break;

		case ClientIndividual:
			headers["ack"] = "client-individual";
			break;

		default:
			headers["ack"] = "auto";
			break;
	}

	StompMessage myMessage("SUBSCRIBE", headers, "");

	auto subscribeMessage = QString(myMessage.toString().c_str());
	QString subscribeFrame = QString(subscribeMessage.data(), subscribeMessage.size() + 1);

	m_webSocket.sendTextMessage(subscribeFrame);
	m_connectionState = Subscribed;
	m_onMessageCallback = onMessageCallback;
}

void QTWebStompClient::closed()
{
	qDebug() << "Connection closed =(";
}

void QTWebStompClient::Ack(const StompMessage & s)
{
	auto ack = s.m_headers.at(std::string("ack"));
}

void QTWebStompClient::Ack(const char* id)
{
	QString ackFrame("ACK\u000Aid:{{TheAckId}}\u000A\u000A\u000A\u0000");
	ackFrame.replace("{{TheAckId}}", id);
	QString ackFrameNullFixed(ackFrame.data(), ackFrame.size() + 1); // solves the null-terminator issue
	m_webSocket.sendTextMessage(ackFrameNullFixed);
}