#include "mywechat.h"
#include <QApplication>
#include"logindialog.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    logindialog logindlg;

    logindlg.show();
    //MyWeChat w;
    //w.show();

    return a.exec();
}
