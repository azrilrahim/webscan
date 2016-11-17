#ifndef PROXYSERVER_H
#define PROXYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <proxyclient.h>
#include <proxyclientthread.h>

class proxyServer : public QObject
{
    Q_OBJECT
public:
    explicit proxyServer(QObject *parent = 0);
    void start();
    ~proxyServer();

private:
    QTcpServer myServer;
    proxyClient *pc;
    proxyClientThread *pct;


signals:

private slots:
    void newConnections();
    void ThreadComplete();

public slots:
};

#endif // PROXYSERVER_H
