#include "DBChat.h"
#include "Login.h"
#include "db.h"
#include <QtWidgets/QApplication>
#include <qmessagebox.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    db db("sqltest");
    if (!db.connect())
    {
        QMessageBox::critical(nullptr, "Database Error", db.getDatabase().lastError().text());
        return -1;
    }

    Login login{db.getDatabaseP()};
    login.show();

    return a.exec();
}
