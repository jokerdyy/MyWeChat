#include "mywechat.h"
#include "ui_mywechat.h"

MyWeChat::MyWeChat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyWeChat)
{
    ui->setupUi(this);
    initMyWeChat();

}




void MyWeChat::initMyWeChat(){
    myUdpSocket = new QUdpSocket(this);
    myUdpPort = 8888；
    myUdpSocket->bind(myUdpPort,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(myUdpSocket,SIGNAL(readyRead()),this,SLOT(recvAndProcessChatMsg()));
//    myfsrv = new FileSrvDlg(this);
//    connect(myfsrv,SIGNAL(sendFileName(QString)),this,SLOT(getSfileName(QString)));//和文件有关读取文件的函数还没有
}

void MyWeChat::on_searchPushButton_clicked(){
    myname = this->windowTitle();
    ui->userLabel->setText(myname);
    sendChatMsg(OnLine);
}




void MyWeChat::sendChatMsg(ChatMsgType msgType, QString rmtName){
    QByteArray qba;
    QDataStream write(&qba,QIODevice::WriteOnly);
    QString locHostIp  = getLocHostIp();
    QString locChatMsg = getLocChatMsg();
    write<<msgType<<myname;
    switch(msgType){

    case ChatMsg:
        write<<locHostIp<<locChatMsg;
        break;
    case OnLine:
        write<<locHostIp;
        break;
    case OffLine:
        break;
    case SfileName:
        write<<locHostIp<<rmtName<<myFileName;
        break;
    case RefFile:
        write<<locHostIp<<rmtName;
        break;
    }
    myUdpSocket->writeDatagram(qba,qba.length(),QHostAddress::Broadcast,myUdpPort);

}


//接受聊天消息
void MyWeChat::recvAndProcessChatMsg(){
    while(myUdpSocket->hasPendingDatagrams()){
         QByteArray qba;
         qba.resize(myUdpSocket->pendingDatagramSize());
         myUdpSocket->readDatagram(qba.data(),qba.size());
         QDataStream read(&qba,QIODevice::ReadOnly);
         int msgType;
        read>>msgType;
        QString name,hostip,chatmsg,rname,fname;
        QString curtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        switch(msgType){
        case ChatMsg:
            read>>name>>hostip>>chatmsg;
            ui->chatTextBrowser->setTextColor(Qt::darkGreen);
            ui->chatTextBrowser->setCurrentFont(QFont("Times New Roman",14));
            ui->chatTextBrowser->append("【"+name+"】"+curtime);
            ui->chatTextBrowser->append(chatmsg);

            break;
        case OnLine:
            read>>name>>hostip;
            onLine(name,curtime);
            break;
        case OffLine:
            read>>name;
            offLine(name,curtime);
            break;
//        case SfileName:
//            read>>name>>hostip>>rname>>fname;
//            recvFileName(name,hostip,rname,fname);
//            break;
//        case RefFile:
//            read>>name>>hostip>>rname;
//            if(myname == rname) myfsrv->cntRefused();
//            break;
        }
    }
}

//会话过程处理
void MyWeChat::onLine(QString name,QString time){
    bool notExist = ui->userListTableWidget->findItems(name,Qt::MatchExactly).isEmpty();
    if(notExist){

        QTableWidgetItem *newuser = new QTableWidgetItem(name);
        int row = ui->userListTableWidget->rowCount();
        ui->userListTableWidget->insertRow(0);

        ui->userListTableWidget->setItem(0,0,newuser);
        ui->chatTextBrowser->setTextColor(Qt::gray);
        ui->chatTextBrowser->setCurrentFont(QFont("Times New Roman",12));
        ui->chatTextBrowser->append(tr("%1 %2 上线！").arg(time).arg(name));
        sendChatMsg(OnLine);
    }

}

void MyWeChat::offLine(QString name,QString time){

    int row = ui->userListTableWidget->findItems(name,Qt::MatchExactly).first()->row();
    ui->userListTableWidget->removeRow(row);
    ui->chatTextBrowser->setTextColor(Qt::gray);
    ui->chatTextBrowser->setCurrentFont(QFont("Times New Roman",12));
    ui->chatTextBrowser->append(tr("%1 %2 下线！").arg(time).arg(name));
    //sendChatMsg(OffLine);
}
void MyWeChat::closeEvent(QCloseEvent *event){
    sendChatMsg(OffLine);
}

//获取ip地址本机以及获取本地消息
QString MyWeChat::getLocHostIp(){
    QList<QHostAddress> addrlist = QNetworkInterface::allAddresses();
    foreach(QHostAddress addr,addrlist){
        if(addr.protocol() == QAbstractSocket::IPv4Protocol) return addr.toString();
    }
    return 0;
}

QString MyWeChat::getLocChatMsg(){
    QString chatmsg = ui->chatTextEdit->toHtml();
    ui->chatTextEdit->clear();
    ui->chatTextEdit->setFocus();
    return chatmsg;
}

//发送按钮
void MyWeChat::on_sendPushButton_clicked(){
    sendChatMsg(ChatMsg);
}





MyWeChat::~MyWeChat()
{
    delete ui;
}
