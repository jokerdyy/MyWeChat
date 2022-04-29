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

    initsql();//打开数据库连接
}
/*
此处实现无边框窗口的移动拖拽
此处需要两个库#include <QMouseEvent>
这里设置的顶部区域+80
*/
void  logindialog::mousePressEvent(QMouseEvent *event)
{
    // 只响应左键
    if (event->button() == Qt::LeftButton)
    {
        QRect titleRect = rect();
        titleRect.setBottom(titleRect.top() + 80);

        if (titleRect.contains(event->pos()))
        {
            m_dragging = true;
            m_startPosition = event->globalPos();
            m_framePosition = frameGeometry().topLeft();
        }
    }

    QWidget::mousePressEvent(event);
}

void logindialog::mouseMoveEvent(QMouseEvent *event)
{
    // 只响应左键
    if (event->buttons() & Qt::LeftButton)
    {
        if (m_dragging)
        {
            // delta 相对偏移量,
            QPoint delta = event->globalPos() - m_startPosition;

            // 新位置：窗体原始位置  + 偏移量
            move(m_framePosition + delta);
        }
    }

    QWidget::mouseMoveEvent(event);
}

void logindialog::mouseReleaseEvent(QMouseEvent * event)
{
    m_dragging = false;
    QWidget::mouseReleaseEvent(event);
}


logindialog::~logindialog()
{
    delete ui;
}
void logindialog::on_loginPushButton_clicked(){
     //qDebug()<<"dianjianniu";


    showWeiChatWindow();


}
//重写enter输入
void logindialog::keyPressEvent(QKeyEvent *ev){
    switch (ev->key()) {
    case Qt::Key_Return:
        on_loginPushButton_clicked();
        break;
    case Qt::Key_Enter:
        on_loginPushButton_clicked();
        break;
    default:
        break;
    }
}


//实现登录
//void logindialog::showWeiChatWindow(){
//    //QFile file("./userlog.xml");
//    //QFile file("C:\\userlog.xml");
//    QFile file("\\\\10.51.70.180\\shared_dir\\userlog.xml");
//    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//          qDebug()<<"错误";
//      }
//    mydoc.setContent(&file);
//    QDomElement root = mydoc.documentElement();
//    if(root.hasChildNodes()){
//        QDomNodeList userList = root.childNodes();
//        bool exist = false;
//        hide_window = false;
//        for(int i=0;i<userList.count();i++){
//            QDomNode user = userList.at(i);
//            QDomNodeList record = user.childNodes();

//            QString uname = record.at(0).toElement().text();
//            QString pword = record.at(1).toElement().text();
//            if(uname == ui->usrLineEdit->text()){
//                exist = true;

//                if(!(pword == ui->pwdLineEdit->text())){

//                    QMessageBox::warning(0,QObject::tr("提示"),"密码错误，请重新输入密码！");
//                    ui->pwdLineEdit->clear();
//                    ui->pwdLineEdit->setFocus();
//                    return;
//                }
//                hide_window = true;
//            }
//        }
//        if(!exist){
//            hide_window = false;
//            QMessageBox::warning(0,QObject::tr("提示"),"用户不存在！请重新输入。");
//            ui->usrLineEdit->clear();
//            ui->pwdLineEdit->clear();
//            ui->usrLineEdit->setFocus();
//            return;
//        }
//        //设置输错误后不会隐藏，输对了隐藏
//        //qDebug()<<"1111";
//        this->hide();
//        weiChatWindow = new MyWeChat(0);
//        //qDebug()<<"1111";
//        weiChatWindow->setWindowTitle(ui->usrLineEdit->text());
//        weiChatWindow->show();
//        //qDebug()<<"showWeixin";


//    }
//}
//去掉导航栏退出
void logindialog::on_btnClose_clicked()
{
    //关闭页面
    this->close();

}

void logindialog::on_btnSet_clicked()
{
    QMessageBox::warning(0,QObject::tr("提示"),"本功能还没实现，请勿点击！");
}



//mysql数据库登录部分
//初始化打开数据库连接
void logindialog::initsql()
{
    /*若输出列表中出现QOCI8和QOCI，则表示驱动加载成功*/
    QStringList drivers = QSqlDatabase::drivers();
    foreach(QString driver, drivers)
    qDebug() <<"\t" << driver;

    //数据库连接
    //本地数据库链接
//    db = QSqlDatabase::addDatabase("QMYSQL");
//    db.setHostName("127.0.0.1");
//    db.setPort(3306);
//    db.setDatabaseName("wechat");
//    db.setUserName("root");
//    db.setPassword("dyy1998");
//    db.open();

    //远程数据库链接
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("180.76.112.152");
    db.setPort(3306);
    db.setDatabaseName("wechat");
    db.setUserName("wechat");
    db.setPassword("@zyh123456");
    db.open();



}
//登录验证账号密码是否正确
void logindialog::showWeiChatWindow()
{
    //获得用户和密码
    QString name=ui->usrLineEdit->text();
    QString password=ui->pwdLineEdit->text();


    QString str=QString("select * from user where username='%1' and passwd='%2'").arg(name).arg(password);
    qDebug()<<str;
    query=new QSqlQuery;
    query->exec(str);

    while (query->next()){
        qDebug()<<query->value(1).toString();
        qDebug()<<query->value(2).toString();
        if(query->value(1).toString() == name && query->value(2).toString()==password){
            //QMessageBox::information(this,"提示","系统登录成功",QMessageBox::Ok);
            this->hide();
            weiChatWindow = new MyWeChat(0);
            weiChatWindow->setWindowTitle(ui->usrLineEdit->text());
            weiChatWindow->show();
            return;
        }
        else{

            qDebug()<<"账号密码不对";
            QMessageBox::warning(0,QObject::tr("提示"),"账号或密码错误，请重新输入！");
            ui->pwdLineEdit->clear();
            ui->pwdLineEdit->setFocus();
            return;

        }
    }
    qDebug()<<"账号密码不对";
    QMessageBox::warning(0,QObject::tr("提示"),"账号或密码错误，请重新输入！");
    ui->pwdLineEdit->clear();
    ui->pwdLineEdit->setFocus();
    return;


}





void logindialog::on_btnTransonly_clicked()
{
    QMessageBox::warning(0,QObject::tr("提示"),"本功能还没实现，请登录传输文件！");
}

void logindialog::on_btnRegister_clicked()
{
    registerWindow = new registerdialog(0);
    registerWindow->show();
    //去掉边框


    //initsql();//不需要重复打开数据库链接

}
