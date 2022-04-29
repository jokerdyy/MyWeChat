#ifndef FILECNTDLG_H
#define FILECNTDLG_H
#include<QFile>
#include<QMessageBox>
#include<QTime>
#include<QHostAddress>
#include<QTcpSocket>
#include<QTcpServer>
#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>
#include<QSqlDriver>
#include <QDialog>
class QTcpSocket;//客户端套接字
namespace Ui {
class FileCntDlg;
}

class FileCntDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FileCntDlg(QWidget *parent = 0);
    ~FileCntDlg();
    void getLocPath(QString lpath);
    void getSrvAddr(QHostAddress saddr);


protected:
    void closeEvent(QCloseEvent *);
private slots:

    void createConnToSrv();//链接服务器
    void readChatMsg();//读取服务器发的文件
    void on_cntClosePushButton_clicked();//点击停止按钮

private:
    Ui::FileCntDlg *ui;
    QTcpSocket *myCntSocket;
    QHostAddress mySrvAddr;
    qint16 mySrvPort;
    qint64 myTotalBytes;
    qint64 myRcvedBytes;
    QByteArray myInputBlock;
    quint16 myBlockSize;

    QFile *myLocPathFile;//接受文件对象的指针
    QString myFileName;//接受文件名
    qint64 myFileNameSize;//文件大小

    QTime mytime;


};

#endif // FILECNTDLG_H
