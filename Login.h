#pragma once

#include "ui_Login.h"
#include <DBChat.h>
#include <QtWidgets/QWidget>
#include <qsqldatabase.h>
class Login : public QWidget
{
    Q_OBJECT

  public:
    Login(QSqlDatabase*db,QWidget *parent = nullptr);
    ~Login();
    QSqlDatabase *_db;

  private:
    Ui::Form ui;
    DBChat *chat = nullptr;

  private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_showRegister_clicked();
};
