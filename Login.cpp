#include "Login.h"
#include <Register.h>
#include <qobject.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>

Login::Login(QSqlDatabase *db,QWidget *parent)
    : QWidget(parent), _db(db)
{
    ui.setupUi(this);
#ifdef _DEBUG // 如果在调试模式下
    ui.lineEdit_id->setText("呵呵哒");
    ui.lineEdit_pwd->setText("123456");
#endif
}

Login::~Login()
{
}

void Login::on_pushButton_login_clicked()
{
    QString id = ui.lineEdit_id->text();
    QString pwd = ui.lineEdit_pwd->text();
    // 判断与数据库中的数据是否匹配
    QSqlQuery query;
    // 用安全的方法
    query.prepare("SELECT * FROM 账户 WHERE userID = :id AND pwd = :pwd");
    query.bindValue(":id", id);
    query.bindValue(":pwd", pwd);
    if (query.exec() && query.next())
    {
        // 登录成功
        ui.label_title->setText("登录成功");
        query.prepare("UPDATE 账户 SET isOnline = 1 WHERE userID = :id");

        chat = new DBChat(id,_db);
        chat->show();
        this->close();
    }
    else
    {
        // 登录失败
        ui.label_title->setText("登录失败,请重新尝试");
    }
}

void Login::on_pushButton_showRegister_clicked()
{
    // 显示对话框,需要注册窗口关闭后，才能继续操作
    Register *registerDialog = new Register(this);
    registerDialog->exec();
    if (registerDialog->registerSuccess)
    {
        ui.label_title->setText("注册成功，请登录");
        QString id = registerDialog->ui.lineEdit_id->text();
        QString pwd = registerDialog->ui.lineEdit_pwd->text();
        ui.lineEdit_id->setText(id);
        ui.lineEdit_pwd->setText(pwd);
    }
    delete registerDialog;
}
