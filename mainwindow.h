#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "login.h"
#include "engine.h"
#include "ledlabel.h"
#include "trade.h"
#include "strategy.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	// 日志输出
	void logOutput(const QString& msg);
	// 初始化槽函数和登录框
	void init();
	// 登录完毕
	void loginDone();
signals:
	// 解决不在ui线程更改ui的问题
	void sendLog(QString);
private slots:
	/* mdApi & tdApi */
	// 统一错误提示框
	void receiveError(QString);

	/* mdApi */
	// md连接状态
	void receiveMdConnectionStatus(bool);
	// md登录成功回调
	void receiveRspLoginMd(CThostFtdcRspUserLoginField);

	/* tdApi */
	// td连接状态
	void receiveTdConnectionStatus(bool);
	// td登录成功回调
	void receiveRspLoginTd(CThostFtdcRspUserLoginField);

	// 接收日志并显示在界面上
	void receiveLog(QString);

	// 接收当前策略状态
	void receiveStrategyStatus(QString);

	// 交易菜单
	void on_trade_triggered();
	// 策略菜单
	void on_strategy_triggered();

	// 测试
	void on_test1_triggered();
	void on_test2_triggered();

private:
	Ui::MainWindow* ui;
	Login login; // 登录界面
	Engine engine; // 管理mdApi和tdApi
	bool isMdLogin = false; // 是否登录行情
	bool isTdLogin = false; // 是否登录交易
	QLabel* strategyStatus; // 底部状态栏的策略状态
	QLabel* userStatus; // 底部状态栏的用户状态
	LedLabel* mdStatus, * tdStatus; // 底部状态栏的行情和交易连接状态
	Trade trade; // 交易界面
	Strategy strategy; // 策略界面
};

#endif // MAINWINDOW_H
