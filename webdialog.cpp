#include "webdialog.h"
#include "ui_webdialog.h"
#include <QDebug>

WebDialog::WebDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WebDialog)
{

    this->proxySvr.start();

    this->netProxy.setType(QNetworkProxy::HttpProxy);
     this->netProxy.setHostName("localhost");
     this->netProxy.setPort(8080);
     QNetworkProxy::setApplicationProxy(this->netProxy);

     ui->setupUi(this);


    //this->netMgr.setProxy(this->netProxy);
    //ui->webV->page()->networkAccessManager()->setProxy(this->netProxy);
    //ui->webV->load(QUrl("http://www.google.com"));
    //this->netMgr.get(QNetworkRequest(QUrl("http://www.google.com")));


   //qDebug() << nMgr.setp
}

WebDialog::~WebDialog()
{
    delete ui;
}

void WebDialog::on_goBtn_clicked()
{
    this->ui->webV->load(QUrl("http://www.facebook.com"));

}
