#ifndef PROXYCLIENT_H
#define PROXYCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QNetworkProxy>

class proxyClient : public QObject
{
    Q_OBJECT
public:
    explicit proxyClient(QObject *parent = 0);
    ~proxyClient();
    void processClient(QTcpSocket *cSock);

private:
    QTcpSocket *cSocket;
    QTcpSocket *eScoket;
    QByteArray cSockData;
    QByteArray eSockData;

    QString getHost(QByteArray httpHeaderPartial);

signals:

private slots:
    void cSockReadyRead();
    void eSockConnected();
    void eSockDisconnected();
    void eSockReadyRead();
    void eSockError (QAbstractSocket::SocketError errorMsg);
public slots:
};

#endif // PROXYCLIENT_H
