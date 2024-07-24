#include "login.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("test");//数据库名称
    db.setUserName("root");//用户名
    db.setPassword(""); // 用户密码
    db.open(); // 连接数据库

    QApplication a(argc, argv);

    login l;
    l.show();

    return a.exec();
}

