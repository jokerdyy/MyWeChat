#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include<QMessageBox>
#include <QMouseEvent>
#include<QKeyEvent>
#include<QDebug>

#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>
#include<QSqlDriver>
namespace Ui {
class registerdialog;
}

class registerdialog : public QDialog
{
    Q_OBJECT

public:
    explicit registerdialog(QWidget *parent = 0);
    ~registerdialog();
protected:
    virtual void keyPressEvent(QKeyEvent *ev);
private slots:
    void on_btnClose2_clicked();

    void on_btnSet2_clicked();

    void on_registerPushButton_clicked();

private:
    Ui::registerdialog *ui;
private:
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);

    bool m_dragging; // 是否正在拖动
    QPoint m_startPosition; // 拖动开始前的鼠标位置
    QPoint m_framePosition; // 窗体的原始位置

    QSqlQuery *query;//注册的sql脚本
};

#endif // REGISTERDIALOG_H
