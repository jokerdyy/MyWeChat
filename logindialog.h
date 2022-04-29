#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include<QDebug>
#include <QDialog>
#include"mywechat.h"
#include"qdom.h"
#include<QFile>
#include<QMessageBox>
#include<QMovie>
#include <QMouseEvent>
#include<QKeyEvent>
namespace Ui {
class logindialog;
}

class logindialog : public QDialog
{
    Q_OBJECT

public:
    explicit logindialog(QWidget *parent = 0);
    ~logindialog();
    void initsql();
private:
    Ui::logindialog *ui;
private slots:
    void on_loginPushButton_clicked();
    void showWeiChatWindow();
    void on_btnClose_clicked();

    void on_btnSet_clicked();
//设置enter确定
protected:
    virtual void keyPressEvent(QKeyEvent *ev);

    //virtual void keyReleaseEvent(QKeyEvent *ev);这里没用到这个这是松开

private:
    MyWeChat * weiChatWindow;
    QDomDocument mydoc;
    //隐藏窗口后设置拖拽移动
private:
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);

    bool m_dragging; // 是否正在拖动
    QPoint m_startPosition; // 拖动开始前的鼠标位置
    QPoint m_framePosition; // 窗体的原始位置
    bool hide_window;

    QSqlDatabase db;//数据库
    QSqlQuery *query;
};

#endif // LOGINDIALOG_H
