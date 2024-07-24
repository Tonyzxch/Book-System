#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    setFixedSize(width(), height()); // 固定窗口大小
    setWindowTitle("书籍上架系统"); // 设置标题
    setWindowIcon(QIcon(":/res/libicon.png")); // 设置图标

    ui->login_btn->setDefault(true); // 设置登录按钮为默认按钮

    // 连接按钮信号到槽函数
    connect(ui->login_btn, &QPushButton::clicked, this, &login::attemptLogin);
    connect(ui->cancel_btn, &QPushButton::clicked, this, &QDialog::close);
    connect(ui->register_btn, &QPushButton::clicked, this, &login::registerUser);
}

login::~login()
{
    delete ui;
}

void login::attemptLogin()
{
    QString username = ui->username_2->text();
    QString password = ui->password_2->text();

    QSqlQuery query;
    query.prepare("SELECT * FROM 用户表 WHERE nickname = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        // 登录成功
        QString userType = query.value("type").toString(); // 获取用户类型

        QMessageBox::information(this, "登录成功", "登录成功！");
        accept(); // 关闭登录对话框

        QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        if (userType == "admin") {
            AdminWindow *adminWindow = new AdminWindow(); // 创建 AdminWindow 对象
            adminWindow->setUserInfo(username, currentTime); // 传递登录信息
            adminWindow->show(); // 显示 AdminWindow
        } else if (userType == "user") {
            UserWindow *userWindow = new UserWindow(); // 创建 UserWindow 对象
            userWindow->setUserInfo(username, currentTime); // 传递登录信息
            userWindow->show(); // 显示 UserWindow
        }
    } else {
        // 登录失败
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
    }
}

void login::registerUser()
{
    QString username = ui->username_2->text();
    QString password = ui->password_2->text();

    // 检查是否同时填写了用户名和密码
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "请填写用户名和密码！");
        return;
    }

    // 检查用户名是否已经存在
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM 用户表 WHERE nickname = :username");
    checkQuery.bindValue(":username", username);
    checkQuery.exec();

    if (checkQuery.next()) {
        QMessageBox::warning(this, "注册失败", "用户名已存在！");
        return;
    }

    // 插入新用户到数据库，默认用户类型为user
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO 用户表 (nickname, password, type) VALUES (:username, :password, 'user')");
    insertQuery.bindValue(":username", username);
    insertQuery.bindValue(":password", password);

    if (insertQuery.exec()) {
        QMessageBox::information(this, "注册成功", "注册成功！");
    } else {
        QMessageBox::warning(this, "注册失败", "注册失败，请重试！错误信息：" + insertQuery.lastError().text());
    }
}
