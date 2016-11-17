#ifndef WEBDIALOG_H
#define WEBDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <proxyserver.h>

namespace Ui {
class WebDialog;
}

class WebDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WebDialog(QWidget *parent = 0);
    ~WebDialog();

private slots:
    void on_goBtn_clicked();

private:
    Ui::WebDialog *ui;
    QNetworkAccessManager netMgr;
    QNetworkProxy netProxy;
    proxyServer proxySvr;
};

#endif // WEBDIALOG_H
