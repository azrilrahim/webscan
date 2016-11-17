#include "proxyserver.h"
#include <QNetworkRequest>

proxyServer::proxyServer(QObject *parent) : QObject(parent)
{

}

void proxyServer::start()
{
    connect (&this->myServer,SIGNAL(newConnection()),this,SLOT(newConnections()));
    if (!this->myServer.listen(QHostAddress::Any,8080)){
        qDebug() << "Server fail";
    }
    qDebug() << "Server start";
}

proxyServer::~proxyServer()
{

}


void proxyServer::newConnections()
{


    this->pct = new proxyClientThread(this->myServer.nextPendingConnection()->socketDescriptor(),this);
    this->pct->run();

}

void proxyServer::ThreadComplete()
{
    //this->pct->deleteLater();
}



