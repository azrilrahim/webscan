#ifndef PROXYCLIENTTHREAD_H
#define PROXYCLIENTTHREAD_H

#include <QObject>
#include <QTcpSocket>
#include <QNetworkProxy>
#include <QThread>
#include <QDebug>
#include <QSslSocket>

struct HTTPHDR{
    QString host;
    quint16 port;
    bool newReq;
};

struct HTTPHDR2{
    quint8 CMD;
    QString CMDi;
    QString HOST;
    quint16 PORT;
    bool status;
};

class proxyClientThread : public QThread
{
    Q_OBJECT

public:
    proxyClientThread(qintptr  sockDesc, QObject *parent = 0);
    ~proxyClientThread();
    void run();
    QByteArray LastResquest;


private:

    QTcpSocket cSocket;
    QTcpSocket eSocket;

    QByteArray cNewReqData;
    QByteArray cSockData;
    QByteArray eSockData;


    void processClient();
    HTTPHDR2 processHeader(QByteArray hdr);

    bool SSLTLS;
    bool INSESSION;

    HTTPHDR getHostInfo(QByteArray httpHeaderPartial);
    int pid;

private slots:
    void cSockReadyRead();
    void eSockConnected();
    void eSockDisconnected();
    void eSockReadyRead();
    void eSockError (QAbstractSocket::SocketError errorMsg);



signals:
    void complete();
};

#endif // PROXYCLIENTTHREAD_H
