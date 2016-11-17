#include "proxyclient.h"

proxyClient::proxyClient(QObject *parent) : QObject(parent)
{
    this->eScoket = new QTcpSocket(this);
    connect (this->eScoket,SIGNAL(connected()),this,SLOT(eSockConnected()));
    connect (this->eScoket,SIGNAL(disconnected()),this,SLOT(eSockDisconnected()));
    connect (this->eScoket,SIGNAL(readyRead()),this,SLOT(eSockReadyRead()));
    connect (this->eScoket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(eSockError(QAbstractSocket::SocketError)));
    this->eScoket->setProxy(QNetworkProxy::NoProxy);
}

proxyClient::~proxyClient()
{
    this->eScoket->deleteLater();
    this->cSocket->deleteLater();
}

void proxyClient::processClient(QTcpSocket *cSock)
{
    this->cSocket = cSock;
    connect(this->cSocket,SIGNAL(readyRead()),this,SLOT(cSockReadyRead()));
}

QString proxyClient::getHost(QByteArray httpHeaderPartial)
{
    QString webDomain;
    QStringList pAddress;
    QStringList serverL;
    bool httpR;

    httpR = false;

    pAddress = QString::QString(httpHeaderPartial).split(" ");

    //check if at 0, has GET or POST
    if (pAddress.at(0).toLower().contains("get")){
        httpR = true;
    }

    if (pAddress.at(0).toLower().contains("post")){
        httpR = true;
    }

    if (!httpR){
        return "";
    }

    webDomain = pAddress.at(1);
    webDomain = webDomain.replace("http://","");
    serverL = webDomain.split("/");
    return serverL.at(0);
}

void proxyClient::cSockReadyRead()
{
    QString tgtHost;
    this->cSockData.clear();

    this->cSockData = this->cSocket->readAll();
    tgtHost = this->getHost(this->cSockData.mid(0,200));

    if (tgtHost.isEmpty()){
        this->cSocket->disconnectFromHost();
        this->cSocket->deleteLater();
        this->eScoket->deleteLater();
        return;
    }

    //connect to host
    qDebug() << "Connecting to" << tgtHost;
    this->eScoket->connectToHost(tgtHost,80);

}

void proxyClient::eSockConnected()
{
    qDebug() << "remote connected. writing data.";
    this->eScoket->write(this->cSockData);

    //wait until eSocket Disconnected

}

void proxyClient::eSockDisconnected()
{
    this->eScoket->disconnectFromHost();
    this->cSocket->disconnectFromHost();
}

void proxyClient::eSockReadyRead()
{
    this->eSockData.clear();
    this->eSockData = this->eScoket->readAll();

    //send back to cSocket
    this->cSocket->write(this->eSockData);
}

void proxyClient::eSockError(QAbstractSocket::SocketError errorMsg)
{
    qDebug() << "eSocket error:" << errorMsg;
}

