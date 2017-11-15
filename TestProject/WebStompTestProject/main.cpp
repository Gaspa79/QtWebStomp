#include <QtCore/QCoreApplication>
#include "QTWebStompClientDll.h"
#include "StompMessage.h"

QTWebStompClient* myClient;

void onMessage(const StompMessage &s)
{
	qDebug() << "The message we got is\r\n" << s.toString().c_str();
	myClient->Ack(s); // you can either specify the entire message as a parameter of ack or just the id you wanna ack.

	myClient->Send("/queue/agent", "this is my infinite loop");
}

void onConnect()
{
	myClient->Subscribe("/queue/agent", onMessage, QTWebStompClient::ClientIndividual); // with type client-individual you'll have to ack every message that's received when you want it removed from the queue.
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	myClient = new QTWebStompClient("ws://10.12.4.142:15674/ws", "ugs", "ugs", onConnect, false);
	return a.exec();

	QTWebStompClient* myClient99 = new QTWebStompClient()
}


