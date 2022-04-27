#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include<QDebug>
#include <QDialog>
#include"mywechat.h"
#include"qdom.h"
#include<QFile>
#include<QMessageBox>
namespace Ui {
class logindialog;
}

class logindialog : public QDialog
{
    Q_OBJECT

public:
    explicit logindialog(QWidget *parent = 0);
    ~logindialog();

private:
    Ui::logindialog *ui;
private slots:
    void on_loginPushButton_clicked();
    void showWeiChatWindow();
private:
    MyWeChat * weiChatWindow;
    QDomDocument mydoc;
};

#endif // LOGINDIALOG_H
