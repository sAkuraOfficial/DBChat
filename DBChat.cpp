#include "DBChat.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTimer>
#include <QtConcurrent>

DBChat::DBChat(QString userID, QSqlDatabase *db, QWidget *parent)
    : QWidget(parent), userID(userID), _db(db)
{
    ui.setupUi(this);
    ui.tableWidget->clear();
    ui.tableWidget->setColumnCount(2);
    ui.tableWidget->setHorizontalHeaderLabels(QStringList() << "姓名"
                                                            << "在线状态");
    ui.tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 上线
    updateUserOnlineStatus(1);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DBChat::updateUserList);
    timer->start(1000); // 每1秒自动刷新用户列表
}

DBChat::~DBChat()
{
    // 确保在析构时停止定时器
    if (timer)
    {
        timer->stop();
        delete timer;
    }
}

void DBChat::closeEvent(QCloseEvent *event)
{
    // 停止定时器
    if (timer)
    {
        timer->stop();
    }
    // 下线
    updateUserOnlineStatus(0);
    event->accept(); // 确保正常关闭事件
}

void DBChat::updateUserOnlineStatus(int status)
{
    QSqlQuery query;
    query.prepare("UPDATE 账户 SET isOnline = :isOnline WHERE userID = :userID");
    query.bindValue(":isOnline", status);
    query.bindValue(":userID", userID);
    if (!query.exec())
    {
        QMessageBox::warning(this, status ? "上线失败" : "下线失败", query.lastError().text());
    }
}

void DBChat::updateUserList()
{
    QtConcurrent::run([this]() {
        // 检查窗口是否仍然有效
        if (!this->isVisible())
            return;

        // 更新自身在线状态
        QSqlQuery query_status(*this->_db);
        query_status.prepare("UPDATE 账户 SET lastActive = :lastActive WHERE userID = :userID");
        query_status.bindValue(":lastActive", QDateTime::currentDateTime());
        query_status.bindValue(":userID", userID);
        query_status.exec(); // 忽略错误处理以简化示例

        // 查询所有用户
        QSqlQuery query(*this->_db);
        query.prepare("SELECT userID, isOnline, lastActive FROM 账户");
        QVector<QStringList> userList;
        query.exec();
        while (query.next())
        {
            if (query.value("userID").toString() == userID)
                continue;

            QString userID = query.value("userID").toString();
            bool isOnline = query.value("isOnline").toBool();
            QDateTime lastActive = query.value("lastActive").toDateTime();
            QString status = isOnline && lastActive.secsTo(QDateTime::currentDateTime()) < 200 ? "在线" : "离线";
            userList.append({userID, status});
        }

        // 更新 UI 在主线程
        QMetaObject::invokeMethod(this, [this, userList]() {
            if (this->isVisible())
            { // 确保窗口仍然可见
                ui.tableWidget->setRowCount(0);
                for (const auto &user : userList)
                {
                    int row = ui.tableWidget->rowCount();
                    ui.tableWidget->insertRow(row);
                    ui.tableWidget->setItem(row, 0, new QTableWidgetItem(user[0]));
                    ui.tableWidget->setItem(row, 1, new QTableWidgetItem(user[1]));
                }
            }
        });
    });
}
