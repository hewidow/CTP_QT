#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSplitter>
#include <QTextEdit>
#include "basetable.h"
#include "login.h"
#include "mdapi.h"
#include "tdapi.h"
#include "quotetable.h"
#include "ledlabel.h"

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
    void receiveLoginField(LoginField); // 登录按钮信号

    /* mdApi */
    void receiveMdConnectionStatus(bool); // md连接状态
    void receiveRspLoginMd(RspLoginField); // md登录成功回调
    void receiveRtnDepthMarketData(QuoteField); // 行情数据

    /* tdApi */
    void receiveTdConnectionStatus(bool); // td连接状态
    void receiveRspLoginTd(RspLoginField); // td登录成功回调
    void receiveAllInstruments(QVector<InstrumentField>); // 获取所有合约id回调

    void receiveLog(QString); // 接收日志并显示在界面上

    void on_action_triggered();
    void on_action_2_triggered();

private:
    Ui::MainWindow *ui;
    Login login; // 登录界面
    MdApi mdApi; // 行情api
    TdApi tdApi; // 交易api
    bool isMdLogin=false; // 是否登录行情
    bool isTdLogin=false; // 是否登录交易
    RspLoginField userInfo; // 用户信息
    QLabel *userStatus; // 用户状态
    LedLabel *mdStatus,*tdStatus; // 行情和交易连接状态
};

#endif // MAINWINDOW_H
