#include "registerdialog.h"
#include "ui_registerdialog.h"

registerdialog::registerdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerdialog)
{
    //去掉边框
    setWindowFlags(Qt::WindowType::FramelessWindowHint);
    ui->setupUi(this);
    ui->registerusername->setFocus();
}


/*
此处实现无边框窗口的移动拖拽
此处需要两个库#include <QMouseEvent>
这里设置的顶部区域+80
*/
void  registerdialog::mousePressEvent(QMouseEvent *event)
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

void registerdialog::mouseMoveEvent(QMouseEvent *event)
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

void registerdialog::mouseReleaseEvent(QMouseEvent * event)
{
    m_dragging = false;
    QWidget::mouseReleaseEvent(event);
}

//以上为窗口移动


//下面实现注册页面enter确定
//重写enter输入
void registerdialog::keyPressEvent(QKeyEvent *ev){
    switch (ev->key()) {
    case Qt::Key_Return:
        on_registerPushButton_clicked();
        break;
    case Qt::Key_Enter:
        on_registerPushButton_clicked();
        break;
    default:
        break;
    }
}




registerdialog::~registerdialog()
{
    delete ui;
}

void registerdialog::on_btnClose2_clicked()
{
    //关闭页面
    this->close();
}

void registerdialog::on_btnSet2_clicked()
{
    QMessageBox::warning(0,QObject::tr("提示"),"本功能还没实现，请勿点击！");
}

void registerdialog::on_registerPushButton_clicked()
{
    //获得用户和密码
    QString name=ui->registerusername->text();
    QString passwd=ui->registerpwd->text();
    QString str=QString("INSERT INTO user VALUES(NULL,'%1','%2')").arg(name).arg(passwd);
    if(!name.isEmpty() && !passwd.isEmpty()){
        query=new QSqlQuery;
        query->exec(str);
        qDebug()<<str;
        QMessageBox::information(this,"提示","注册成功！",QMessageBox::Ok);
        this->hide();
    }
    else{
        ui->registerusername->clear();
        ui->registerpwd->clear();
        ui->registerusername->setFocus();
        QMessageBox::information(this,"提示","存在输入为空，注册失败！请重新注册！",QMessageBox::Ok);
    }


}
