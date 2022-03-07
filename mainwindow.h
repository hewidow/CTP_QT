#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "login.h"
#include "engine.h"
#include "ledlabel.h"
#include "trade.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void logOutput(const QString &msg); // 日志输出
    void init(); // 初始化槽函数和登录框
    void loginDone(); // 登录完毕
signals:
    void sendLog(QString); // 解决不在ui线程更改ui的问题
private slots:
    /* mdApi & tdApi */
    void receiveError(QString); // 统一错误提示框

    /* mdApi */
    void receiveMdConnectionStatus(bool); // md连接状态
    void receiveRspLoginMd(CThostFtdcRspUserLoginField); // md登录成功回调

    /* tdApi */
    void receiveTdConnectionStatus(bool); // td连接状态
    void receiveRspLoginTd(CThostFtdcRspUserLoginField); // td登录成功回调

    void receiveLog(QString); // 接收日志并显示在界面上

    void on_action_triggered();
    void on_action_2_triggered();

    void on_action_5_triggered();

private:
    Ui::MainWindow *ui;
    Login login; // 登录界面
    Engine engine; // 管理mdApi和tdApi
    bool isMdLogin=false; // 是否登录行情
    bool isTdLogin=false; // 是否登录交易
    CThostFtdcRspUserLoginField userInfo; // 用户信息
    QLabel *userStatus; // 底部状态栏的用户状态
    LedLabel *mdStatus,*tdStatus; // 底部状态栏的行情和交易连接状态
    Trade trade; // 交易界面
};

#endif // MAINWINDOW_H
