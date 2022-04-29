#ifndef MYWECHAT_H
#define MYWECHAT_H

#include <QDialog>
#include<QMessageBox>
#include<QUdpSocket>
#include<QNetworkInterface>
#include<QDateTime>
#include<QFile>
#include<QFileDialog>
#include"qdom.h"
#include"filesrvdlg.h"
#include"filecntdlg.h"
#include<QKeyEvent>

class FileSrvDlg;
namespace Ui {
class MyWeChat;
}
enum ChatMsgType { ChatMsg,OnLine,OffLine,SfileName,RefFile};
class MyWeChat : public QDialog
{
    Q_OBJECT

public:
    explicit MyWeChat(QWidget *parent = 0);
    ~MyWeChat();
    void initMyWeChat();
    void onLine(QString name,QString time);
    void offLine(QString name,QString time);
    void sendChatMsg(ChatMsgType msgType,QString rmtName ="");

    QString getLocHostIp();
    QString getLocChatMsg();
    void recvFileName(QString name,QString hostip,QString rmtname,QString filename);


protected:
    void closeEvent(QCloseEvent *event);
    //bool eventFilter(QObject *target, QEvent *event);//事件过滤器diyizhong fangfa
    //virtual void keyPressEvent(QKeyEvent *ev);//enter输入第二种方法

private slots:
    void on_sendPushButton_clicked();
    void recvAndProcessChatMsg();
    void on_searchPushButton_clicked();
    void getSfileName(QString);
    void on_transPushButton_clicked();

private:
    Ui::MyWeChat *ui;
    QString myname = "";
    QUdpSocket *myUdpSocket;
    qint16 myUdpPort;
    QDomDocument myDoc;
    QString myFileName;
    FileSrvDlg *myfsrv;


};

#endif // MYWECHAT_H
