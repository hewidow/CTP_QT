#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    init();
}

Login::~Login()
{
    delete ui;
}
void Login::init()
{
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
}
void Login::on_loginBtn_clicked()
{
    emit sendLoginField(LoginField{
        ui->mdEdit->text(),
        ui->tdEdit->text(),
        ui->brokeIdEdit->text(),
        ui->appIdEdit->text(),
        ui->authCodeEdit->text(),
        ui->userIdEdit->text(),
        ui->passwordEdit->text()
    });
}
