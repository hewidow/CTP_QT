#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "global.h"
#include "debug.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    void init();
signals:
    void sendLoginField(LoginForm);
private slots:
    void on_loginBtn_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
