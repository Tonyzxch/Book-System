#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QString>
#include <QSqlDatabase>
#include "adminwindow.h"
#include "userwindow.h"

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void attemptLogin();
    void registerUser();

private:
    Ui::login *ui;
};

#endif // LOGIN_H
