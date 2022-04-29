#include "filecntdlg.h"
#include "ui_filecntdlg.h"

FileCntDlg::FileCntDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileCntDlg)
{
    ui->setupUi(this);
    myCntSocket = new QTcpSocket(this);
    mySrvPort = 8888;
    connect(myCntSocket,SIGNAL(readyRead()),this,SLOT(readChatMsg()));
    myFileName = "";
    myFileNameSize = 0;
    myTotalBytes = 0;
    myRcvedBytes = 0;
}
//通过createConnToSrv链接服务器
void FileCntDlg::createConnToSrv(){
    myBlockSize = 0;
    myCntSocket->abort();
    myCntSocket->connectToHost(mySrvAddr,mySrvPort);
    mytime.start();
}
//readchatmsgjieshou文件数据
void FileCntDlg::readChatMsg(){
    QDataStream in(myCntSocket);
    in.setVersion((QDataStream::Qt_5_9));
    float usedTime = mytime.elapsed();
    if(myRcvedBytes<=sizeof(qint64)*2){
        if((myCntSocket->bytesAvailable()>=sizeof(qint64)*2) && (myFileNameSize==0)){
            in>>myTotalBytes>>myFileNameSize;
            myRcvedBytes += sizeof(qint64)*2;
        }
        if((myCntSocket->bytesAvailable()>=myFileNameSize) && (myFileNameSize!=0)){
            in>>myFileName;
            myRcvedBytes +=myFileNameSize;
            myLocPathFile->open(QFile::WriteOnly);
            ui->rfileNameLineEdit->setText(myFileName);
        }else{
            return;
        }
    }
    if(myRcvedBytes< myTotalBytes){
        myRcvedBytes +=myCntSocket->bytesAvailable();
        myInputBlock = myCntSocket->readAll();
        myLocPathFile->write(myInputBlock);
        myInputBlock.resize(0);
    }
    ui->recvProgressBar->setMaximum(myTotalBytes);
    ui->recvProgressBar->setValue(myRcvedBytes);
    double transpeed = myRcvedBytes/usedTime;
    ui->rfileSizeLineEdit->setText(tr("%1").arg(myTotalBytes/(1024*1024))+"MB");
    ui->recvSizeLineEdit->setText(tr("%1").arg(myRcvedBytes/(1024*1024))+"MB");
    ui->rateLabel->setText(tr("%1").arg(transpeed*1000/(1024*1024),0,'f',2)+"MB/秒");
    if(myRcvedBytes == myTotalBytes){
        myLocPathFile->close();
        myCntSocket->close();
ui->rateLabel->setText("接受完成！");
    }

}

//客户端停止clicked
void FileCntDlg::on_cntClosePushButton_clicked(){
    myCntSocket->abort();
    myLocPathFile->close();
    close();
}

void FileCntDlg::closeEvent(QCloseEvent *){
    on_cntClosePushButton_clicked();
}

void FileCntDlg::getLocPath(QString lpath){
    myLocPathFile = new QFile(lpath);
}
void FileCntDlg::getSrvAddr(QHostAddress saddr){
    mySrvAddr = saddr;
    createConnToSrv();
}

FileCntDlg::~FileCntDlg()
{
    delete ui;
}
