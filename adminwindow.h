#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QPixmap>
#include <QSqlError>
#include <QInputDialog>
#include <QFileDialog>
#include <QString>
#include <QTimer>
#include <QDateTime>
#include <QFont>
#include "imagedialog.h"
#include "login.h"

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = 0);
    ~AdminWindow();

    void setupTableView(QTableView *tableView); // 隐藏表头
    void setupTableView2(); // 设置窗口字体

    // 状态栏，用于显示用户信息和当前时间
    void setUserInfo(const QString &username, const QString &currentTime);
    QString username;
    QString currentTime;
    QTimer *timeUpdateTimer;
    void updateTime();

    // 点击按钮，展示窗口
    void showBooksTable();
    void showUsersTable();
    void showRecordsTable();
    void showWorkTable();

    // 增、删、改功能键
    void addBook();
    void removeBook();
    void modifyBook();

    void addUser();
    void removeUser();
    void modifyUser();

    void addRecord();
    void removeRecord();
    void modifyRecord();

    void addWork();
    void removeWork();
    void modifyWork();

    // 显示图书图片
    void onBookCellClicked(const QModelIndex &index);

    // 退出及重新登陆按钮
    void onLogoutButtonClicked();
    void onReloginButtonClicked();

    // 排序和筛选功能
    void showFilterMenu(const QPoint &pos);
    void filterBooksByAuthor();
    void filterBooksByType1();
    void filterBooksByStatus();
    void filterBooksByFloor();
    void sortBooksByPrice(bool ascending);
    void updateTableView(QSqlQueryModel *queryModel);

private:
    Ui::AdminWindow *ui;
};

#endif // ADMINWINDOW_H
