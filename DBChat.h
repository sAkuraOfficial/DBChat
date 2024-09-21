#pragma once

#include "ui_DBChat.h"
#include <QDateTime>
#include <QSqlDatabase>
#include <QTimer>
#include <QtWidgets/QWidget>

class DBChat : public QWidget
{
    Q_OBJECT

  public:
    DBChat(QString userID, QSqlDatabase *db, QWidget *parent = nullptr);
    ~DBChat();

  protected:
    void closeEvent(QCloseEvent *event) override;

  private:
    void updateUserOnlineStatus(int status); // �����һ��
    void updateUserList();

    Ui::DBChatClass ui;
    QString userID;
    QSqlDatabase *_db;
    QTimer *timer;
  
};
