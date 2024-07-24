#ifndef USERWINDOW_H
#define USERWINDOW_H

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
#include <QRegExp>
#include <QString>
#include <QTimer>
#include <QDateTime>
#include "imagedialog.h"
#include "login.h"

namespace Ui {
class UserWindow;
}

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(QWidget *parent = 0);
    ~UserWindow();

    void setupTableView(QTableView *tableView); // 隐藏表头
    void setupTableView2(); // 设置窗口字体

    // 状态栏，用于显示用户信息和当前时间
    void setUserInfo(const QString &username, const QString &currentTime);
    int getUserIdFromUsername(const QString& username);
    QString username;
    QString currentTime;
    QTimer *timeUpdateTimer;
    void updateTime();

    // 点击按钮，展示窗口
    void showBooksTable();
    void showWorkTable();
    void showRecordsTable();

    // 增、删、改功能键
    void addWork();
    void removeWork();
    void modifyWork();

    void addRecord();
    void removeRecord();
    void modifyRecord();

    // 显示图书图片
    void onBookCellClicked(const QModelIndex &index);

    // 退出及重新登陆按钮
    void onLogoutButtonClicked();
    void onReloginButtonClicked();

private:
    Ui::UserWindow *ui;
};

#endif // USERWINDOW_H
