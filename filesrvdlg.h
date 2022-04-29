#ifndef FILESRVDLG_H
#define FILESRVDLG_H
#include<QFile>
#include<QMessageBox>
#include<QTime>
#include<QFileDialog>
#include<QTcpSocket>
#include<QTcpServer>
#include<QUdpSocket>
#include <QDialog>
class QFile;
class QTcpServer;
class QTcpSocket;

namespace Ui {
class FileSrvDlg;
}

class FileSrvDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FileSrvDlg(QWidget *parent = 0);
    ~FileSrvDlg();
    void cntRefused();//被客户端拒绝
protected:
    void closeEvent(QCloseEvent *);
private slots:
    void sndChatMsg(); //发送消息
    void refreshProgress(qint64 bynum);//刷新服务器进度条
    void on_openFilePushButton_clicked();//dakai 选择要传输的文件
    void on_sendFilePushButton_clicked();//发送按钮
    void on_srvClosePushButton_clicked();//停止按钮事件

private:
    Ui::FileSrvDlg *ui;
    QTcpServer *myTcpSrv;
    QTcpSocket *mySrvSocket;
    qint16 mySrvPort;

    QFile *myLocPathFile;
    QString myPathFile;//路径
    QString myFileName;//文件名

    qint64 myTotalBytes;
    qint64 mySendBytes;
    qint64 myBytesTobeSend;
    qint64 myPayloadSize;
    QByteArray myOutputBlock;
    QTime mytime;
signals:
    void sendFileName(QString name);

};

#endif // FILESRVDLG_H
