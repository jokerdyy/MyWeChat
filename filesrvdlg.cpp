#include "filesrvdlg.h"
#include "ui_filesrvdlg.h"

FileSrvDlg::FileSrvDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSrvDlg)
{
    ui->setupUi(this);
    myTcpSrv = new QTcpServer(this);
    mySrvPort = 8888;
    connect(myTcpSrv,SIGNAL(newConnection()),this,SLOT(sndChatMsg()));
    myTcpSrv->close();
    myTotalBytes = 0;
    myBytesTobeSend = 0;
    mySendBytes = 0;
    myPayloadSize = 64*1024;
    ui->sendProgressBar->reset();
    ui->openFilePushButton->setEnabled(true);
    ui->sendFilePushButton->setEnabled(false);
}

//发送数据
void FileSrvDlg::sndChatMsg(){
    ui->sendFilePushButton->setEnabled(false);
    mySrvSocket = myTcpSrv->nextPendingConnection();
    connect(mySrvSocket,SIGNAL(bytesWritten(qint64)),this,SLOT(refreshProgress(qint64)));
    myLocPathFile = new QFile(myPathFile);
    myLocPathFile->open(QFile::ReadOnly);
    myTotalBytes = myLocPathFile->size();
    QDataStream sendOut(&myOutputBlock,QIODevice::WriteOnly);
    sendOut.setVersion((QDataStream::Qt_5_9));
    mytime.start();
    QString curFile = myPathFile.right(myPathFile.size()-myPathFile.lastIndexOf('/') -1);
    sendOut<<qint64(0)<<qint64(0)<<curFile;
    myTotalBytes+=myOutputBlock.size();
    sendOut.device()->seek(0);
    sendOut<<myTotalBytes<<qint64(myOutputBlock.size() - sizeof(qint64) *2);
    myBytesTobeSend = myTotalBytes - mySrvSocket->write(myOutputBlock);
    myOutputBlock.resize(0);
}

//open文件按钮
void FileSrvDlg::on_openFilePushButton_clicked(){
    myPathFile = QFileDialog::getOpenFileName(this);
    if(!myPathFile.isEmpty()){
        myFileName = myPathFile.right(myPathFile.size()-myPathFile.lastIndexOf('/')-1);
        ui->sfileNameLineEdit->setText(tr("%1").arg(myFileName));
        ui->sendFilePushButton->setEnabled(true);
        ui->openFilePushButton->setEnabled(false);
    }
}
//发送按钮
void FileSrvDlg::on_sendFilePushButton_clicked(){
    if(!myTcpSrv->listen(QHostAddress::Any,mySrvPort)){
       QMessageBox::warning(0,QObject::tr("异常"),"请检查网络链接！");
        close();
        return;
    }
    emit sendFileName(myFileName);
}

//停止按钮
void FileSrvDlg::on_srvClosePushButton_clicked(){
    if(myTcpSrv->isListening()){
        myTcpSrv->close();
        myLocPathFile->close();
        mySrvSocket->abort();
    }

    close();
}
//突然关闭页面
void FileSrvDlg::closeEvent(QCloseEvent *){
    on_srvClosePushButton_clicked();
}

//刷新进度条
void FileSrvDlg::refreshProgress(qint64 bynum){
    qApp->processEvents();
    mySendBytes +=(int) bynum;
    if(myBytesTobeSend>0){
        myOutputBlock = myLocPathFile->read(qMin(myBytesTobeSend,myPayloadSize));
    myBytesTobeSend -=(int)mySrvSocket->write(myOutputBlock);
    myOutputBlock.resize(0);
    }else{
        myLocPathFile->close();
    }
    ui->sendProgressBar->setMaximum(myTotalBytes);
    ui->sendProgressBar->setValue(mySendBytes);
    ui->sfileSizeLineEdit->setText(tr("%1").arg(myTotalBytes/(1024*1024))+"MB");
    ui->sendSizeLineEdit->setText(tr("%1").arg(mySendBytes/(1024*1024))+"MB");

    if(mySendBytes == myTotalBytes){
        myLocPathFile->close();
        myTcpSrv->close();
        QMessageBox::information(0,QObject::tr("完毕"),"文件传输完成！");


    }

}

void FileSrvDlg::cntRefused(){
    myTcpSrv->close();
    QMessageBox::warning(0,QObject::tr("提示"),"对方拒绝接受！");
}

FileSrvDlg::~FileSrvDlg()
{
    delete ui;
}
