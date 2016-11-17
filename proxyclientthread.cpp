#include "proxyclientthread.h"

proxyClientThread::proxyClientThread(qintptr sockDesc, QObject *parent) : QThread(parent)
{

    this->SSLTLS = false;
    this->INSESSION = false;

    this->pid = sockDesc;
    //connect (&this->eSocket,SIGNAL(connected()),this,SLOT(eSockConnected()));
    connect (&this->eSocket,SIGNAL(disconnected()),this,SLOT(eSockDisconnected()));
    connect (&this->eSocket,SIGNAL(readyRead()),this,SLOT(eSockReadyRead()));
    connect (&this->eSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(eSockError(QAbstractSocket::SocketError)));
    this->eSocket.setProxy(QNetworkProxy::NoProxy);

    this->cSocket.setSocketDescriptor(sockDesc);
    connect(&this->cSocket,SIGNAL(disconnected()),this,SLOT(eSockDisconnected()));
    connect(&this->cSocket,SIGNAL(readyRead()),this,SLOT(cSockReadyRead()),Qt::DirectConnection);



}

proxyClientThread::~proxyClientThread()
{

}

void proxyClientThread::run()
{

    qDebug() << "Thread" << this->pid << "Started";
    this->SSLTLS = false;

}

void proxyClientThread::processClient()
{
    HTTPHDR2 pHead;

    //recieved incoming client packet
    this->cSockData.clear();
    this->cSockData = this->cSocket.readAll();

    if (this->eSocket.state() == QAbstractSocket::ConnectedState){
        if (this->SSLTLS){
            eSocket.write(cSockData);
            return;
        }

        eSocket.write(cSockData);
        return;
    }


    //process header
    pHead = this->processHeader(cSockData.mid(0,100));
    if (!pHead.status){
        return;
    }

    //qDebug() << "Processing" << pHead.CMD << pHead.HOST << pHead.PORT;

    //process SSL/TLS Connection;
    if (pHead.CMD == 3){
         if (eSocket.state() == QAbstractSocket::UnconnectedState){
             //this->SSLTLS = true;
             eSocket.connectToHost(pHead.HOST,pHead.PORT);
             if (!eSocket.waitForConnected()){
                 return;
             }
             this->SSLTLS = true;
             //send reply message to client
             this->eSockData.clear();
             this->eSockData.append("HTTP/1.0 200 OK\n\n");
             qDebug() << "writing connection SSL response";
             this->cSocket.write(this->eSockData);
             return;
         }
    }

    //process other HTML Command
    if (eSocket.state() == QAbstractSocket::UnconnectedState){
        this->SSLTLS = false;
        qDebug() << "Connecting:" << pHead.CMD << pHead.HOST << pHead.PORT;
        qDebug() << "Thread" << this->pid << pHead.CMDi << "Connecting" << pHead.HOST << ":" <<pHead.PORT;
        eSocket.connectToHost(pHead.HOST,pHead.PORT);
        if (!eSocket.waitForConnected()){
            return;
        }
        qDebug() << "Thread" << this->pid << pHead.CMDi << "Connected";
        eSocket.write(cSockData);
        return;
    }

    return;
}

HTTPHDR2 proxyClientThread::processHeader(QByteArray hdr)
{

    QStringList mLine;
    QString data;
    HTTPHDR2 hdr2;
    QString host;
    QStringList hostL;

    //qDebug() << "PROCESS HEADER STARTED";

    hdr2.CMD = 0;
    hdr2.HOST.clear();
    hdr2.PORT = 80;
    hdr2.status = false;

    data = QString::QString(hdr).trimmed();
    mLine = data.split('\n').at(0).split(" ");
    if (mLine.size() < 3){
        return hdr2;
    }

    //get command;
    if (mLine.at(0).toLower().contains("get")){
        hdr2.CMD = 1;
        hdr2.CMDi = "GET";
    }
    if (mLine.at(0).toLower().contains("post")){
        hdr2.CMD = 2;
        hdr2.CMDi = "POST";
    }
    if (mLine.at(0).toLower().contains("connect")){
        hdr2.CMD = 3;
        hdr2.CMDi = "SSL";
    }

    //get host
    host = mLine.at(1).trimmed();
    host = host.replace("http://","");
    host = host.replace("https://","");
    hostL = host.split("/").at(0).split(":");

    if (hostL.size() == 2){
        hdr2.HOST = hostL.at(0);
        hdr2.PORT = hostL.at(1).toInt();
    }

    hdr2.HOST = hostL.at(0);
    hdr2.status = true;

    return hdr2;

}

void proxyClientThread::cSockReadyRead()
{

    this->processClient();
    return;    
}


void proxyClientThread::eSockConnected()
{
    qDebug() << "Connected";
    if (this->SSLTLS){
        //write a reply for SSL established
        qDebug() << "Sending SSL/TLS enable";
        this->eSockData.clear();
        this->eSockData.append("HTTP/1.0 200 OK\n\n");
        qDebug() << "writing connection SSL response";
        //qDebug() << eSockData << "==";
        //eSocket.write(OLD);
        this->cSocket.write(this->eSockData);
        return;
    }

    //others
    this->LastResquest = this->cSockData;
    this->eSocket.write(this->cSockData);
}

void proxyClientThread::eSockDisconnected()
{
    this->eSocket.disconnectFromHost();
    this->cSocket.disconnectFromHost();
    qDebug() << "Thread" << this->pid << "Stop";
    this->exit(0);

}

void proxyClientThread::eSockReadyRead()
{
    this->eSockData.clear();
    this->eSockData = this->eSocket.readAll();
    this->cSocket.write(this->eSockData);
}

void proxyClientThread::eSockError(QAbstractSocket::SocketError errorMsg)
{
    qDebug() << "eSocket error:" << errorMsg;
}



HTTPHDR proxyClientThread::getHostInfo(QByteArray httpHeaderPartial)
{
    QString webDomain;
    QStringList webDomainL;
    QStringList pAddress;
    QStringList svrCmdL;
    QStringList serverL;
    bool httpR;

    HTTPHDR rs;

    rs.host = "";
    rs.port = 0;
    rs.newReq = false;

    qDebug() << "PID" << this->pid << ":" << httpHeaderPartial;
    httpR = false;
    webDomain.clear();

    pAddress = QString::QString(httpHeaderPartial).split('\n');
    svrCmdL = pAddress.at(0).split(" ");


    if (svrCmdL.at(0).toLower().contains("post")){
        rs.newReq = true;
    }

    if (svrCmdL.at(0).toLower().contains("get")){
        rs.newReq = true;
    }

    webDomain = svrCmdL.at(1);
    webDomain = webDomain.replace("http://","");
    webDomain = webDomain.replace("https://","");
    webDomainL = webDomain.split("/");
    serverL = webDomainL.at(0).split(":");

    if (serverL.size() == 2){
        rs.host = serverL.at(0);
        rs.port = serverL.at(1).toInt();
    }
    else
    {
        rs.host = serverL.at(0);
        rs.port = 80;
    }

    return rs;
}


