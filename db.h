#pragma once

#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

class db {
public:
    db(const QString& dsn);
    ~db();

    bool connect();
    void disconnect();
    QSqlDatabase getDatabase();
    QSqlDatabase* getDatabaseP();

private:
    QString dsn;          // 数据源名称
    QSqlDatabase database; // 数据库对象
};
