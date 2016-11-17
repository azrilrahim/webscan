#include "webdialog.h"
#include <QLoggingCategory>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLoggingCategory::setFilterRules("qt.network.ssl.w arning=false");
    WebDialog w;
    w.show();

    return a.exec();
}
