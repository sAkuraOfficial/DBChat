#pragma once

#include "ui_DBChat.h"
#include <QDateTime>
#include <QtWidgets/QWidget>
#include <qtimer.h>
#include<qsqldatabase.h>
class DBChat : public QWidget
{
    Q_OBJECT

  public:
    DBChat(QString userID, QSqlDatabase *db, QWidget *parent = nullptr);
    ~DBChat();
    QSqlDatabase *_db;

  private:
    Ui::DBChatClass ui;
    QString userID;
    QTimer *timer;
  private slots:
    void updateUserList();
    // void on_pushButton_getData_clicked();
  protected:
    void closeEvent(QCloseEvent *event);
};
