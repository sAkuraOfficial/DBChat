#include "Register.h"
#include "qsqldatabase.h"
#include "qsqlquery.h"
Register::Register(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

Register::~Register()
{
}

void Register::on_pushButton_register_clicked()
{
    QString userID = ui.lineEdit_id->text();
    QString pwd = ui.lineEdit_pwd->text();
    QSqlQuery query;
    // 先判断是否已经存在
    query.prepare("SELECT * FROM 账户 WHERE userID = :id");
    query.bindValue(":id", userID);
    if (query.exec() && query.next())
    {
        ui.label_title->setText("用户已存在");
        return;
    }
    // 插入数据
    query.prepare("INSERT INTO 账户(userID, pwd) VALUES(:id, :pwd)");
    query.bindValue(":id", userID);
    query.bindValue(":pwd", pwd);
    if (query.exec())
    {
        ui.label_title->setText("注册成功");
        registerSuccess = true;
        this->close();
    }
    else
    {
        ui.label_title->setText("注册失败");
        registerSuccess = false;
    }
}