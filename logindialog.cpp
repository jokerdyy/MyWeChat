#include "logindialog.h"
#include "ui_logindialog.h"

logindialog::logindialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::logindialog)
{
    //去掉边框
    setWindowFlags(Qt::WindowType::FramelessWindowHint);
    ui->setupUi(this);
    ui->pwdLineEdit->setFocus();
    //connect(loginPushButton,SIGNAL(clicked()),this,SLOT(on_loginPushButton_clicked()));
}

logindialog::~logindialog()
{
    delete ui;
}

void logindialog::on_loginPushButton_clicked(){
     qDebug()<<"dianjianniu";
    showWeiChatWindow();
}

//实现登录
void logindialog::showWeiChatWindow(){
    QFile file("E:\\Qt\\WeChat\\MyWeChat\\userlog.xml");
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
          qDebug()<<"错误";
      }
    mydoc.setContent(&file);
    QDomElement root = mydoc.documentElement();
    if(root.hasChildNodes()){
        QDomNodeList userList = root.childNodes();
        bool exist = false;
        for(int i=0;i<userList.count();i++){
            QDomNode user = userList.at(i);
            QDomNodeList record = user.childNodes();

            QString uname = record.at(0).toElement().text();
            QString pword = record.at(1).toElement().text();
            if(uname == ui->usrLineEdit->text()){
                exist = true;
                if(!(pword == ui->pwdLineEdit->text())){
                    QMessageBox::warning(0,QObject::tr("提示"),"密码错误，请重新输入密码！");
                    ui->pwdLineEdit->clear();
                    ui->pwdLineEdit->setFocus();
                    return;
                }
            }
        }
        if(!exist){
            QMessageBox::warning(0,QObject::tr("提示"),"用户不存在！请重新输入。");
            ui->usrLineEdit->clear();
            ui->pwdLineEdit->clear();
            ui->usrLineEdit->setFocus();
            return;
        }
        weiChatWindow = new MyWeChat(0);
        weiChatWindow->setWindowTitle(ui->usrLineEdit->text());
        weiChatWindow->show();
    }
}
