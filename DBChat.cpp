#include "DBChat.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtConcurrent>
DBChat::DBChat(QString userID, QSqlDatabase *db, QWidget *parent)
    : QWidget(parent), userID(userID), _db(db)
{

    ui.setupUi(this);
    ui.tableWidget->clear();
    ui.tableWidget->setColumnCount(2);
    ui.tableWidget->setHorizontalHeaderLabels(QStringList() << "姓名"
                                                            << "在线状态");
    // 关闭水平滚动条
    ui.tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 设置表头自适应宽度
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 上线
    QSqlQuery query;
    query.prepare("UPDATE 账户 SET isOnline = 1 WHERE userID = :userID");
    query.bindValue(":userID", userID);
    if (!query.exec())
    {
        QMessageBox::warning(this, "上线失败", query.lastError().text());
        // 关闭窗口
        this->close();
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DBChat::updateUserList);
    timer->start(1000); // 每10秒自动刷新用户列表

    updateUserList(); // 手动刷新一次
}

DBChat::~DBChat()
{
}

void DBChat::closeEvent(QCloseEvent *event)
{
    // 下线
    QSqlQuery query;
    query.prepare("UPDATE 账户 SET isOnline = 0 WHERE userID = :userID");
    query.bindValue(":userID", userID);
    if (!query.exec())
    {
        QMessageBox::warning(this, "下线失败", query.lastError().text());
    }
}

void DBChat::updateUserList()
{
    QtConcurrent::run([this]() {
        // 更新自身在线状态
        QSqlQuery query_status = QSqlQuery(*this->_db);
        query_status.prepare("UPDATE 账户 SET lastActive = :lastActive WHERE userID = :userID");
        query_status.bindValue(":lastActive", QDateTime::currentDateTime());
        query_status.bindValue(":userID", userID);
        query_status.exec(); // 忽略错误处理以简化示例

        // 查询所有用户
        QSqlQuery query = QSqlQuery(*this->_db); 
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
            QString status;

            if (isOnline)
                status = (lastActive.secsTo(QDateTime::currentDateTime()) < 200) ? "在线" : "离线";
            else
                status = "离线";

            userList.append({userID, status});
        }

        // 更新 UI 在主线程
        QMetaObject::invokeMethod(this, [this, userList]() {
            ui.tableWidget->setRowCount(0);
            for (const auto &user : userList)
            {
                int row = ui.tableWidget->rowCount();
                ui.tableWidget->insertRow(row);
                ui.tableWidget->setItem(row, 0, new QTableWidgetItem(user[0]));
                ui.tableWidget->setItem(row, 1, new QTableWidgetItem(user[1]));
            }
        });
    });
}