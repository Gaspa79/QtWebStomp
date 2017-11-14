#include <QtCore/QCoreApplication>
#include "QTWebStompClientDll.h"

QTWebStompClient* myClient;

void onMessage(const StompMessage &s)
{
	qDebug() << "Hopefully this works \r\n" << s.toString().c_str();
	qDebug() << s.m_headers.at(std::string("ack")).c_str();
	qDebug() << "Acking message";
	myClient->Ack(s);
}

void actualFunctionToBeCalled()
{
	myClient->Subscribe("/queue/agent", onMessage, QTWebStompClient::ClientIndividual);
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QUrl myUrl(QStringLiteral("ws://10.12.4.142:15674/ws"));
	myClient = new QTWebStompClient(myUrl, "ugs", "ugs", actualFunctionToBeCalled, true);
	
	return a.exec();
}


