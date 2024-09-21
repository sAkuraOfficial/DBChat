#include "db.h"
#include <QMessageBox>
db::db(const QString& dsn) : dsn(dsn) {
	database = QSqlDatabase::addDatabase("QODBC");
	//database.setDatabaseName(dsn);
	database.setDatabaseName("Driver={ODBC Driver 17 for SQL Server};Server=haohao.iok.la,23693;Database=DBChat;UID=sa;PWD=123456;");

}

db::~db() {
	disconnect(); // 确保在析构时断开连接
}

bool db::connect() {
	if (!database.open()) {
		QMessageBox::critical(nullptr, "Database Error", database.lastError().text());
		return false;
	}
	return true;
}

void db::disconnect() {
	if (database.isOpen()) {
		database.close();
	}
}

QSqlDatabase db::getDatabase() {
	return database;
}
QSqlDatabase* db::getDatabaseP() {
	return &database;
}
