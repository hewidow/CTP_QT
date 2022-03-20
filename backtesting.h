#pragma once

#include <QDialog>
#include <QCloseEvent>
#include <QDir>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QThread>
#include "ui_backtesting.h"
#include "util.h"
#include "debug.h"
#include "BacktestingEngine.h"

class Backtesting : public QDialog
{
	Q_OBJECT

public:
	Backtesting(QWidget *parent = Q_NULLPTR);
	~Backtesting();
	// 显示可选数据库
	void showDatabase();
protected:
	// 重写关闭事件
	void closeEvent(QCloseEvent* event) override;
private slots:
	// 显示错误弹窗
	void receiveError(QString);
	// 回测结束
	void endBacktestingEngine(BacktestingResult);
	// 开始回测按钮
	void on_start_clicked();
	// 暂停回测按钮
	void on_stop_clicked();
signals:
	// 发送回测开始还是暂停的状态
	void sendBacktestingStatus(bool);
	// 显示回测结果
	void sendBacktestingResult(BacktestingResult);
	// 回测开始
	void startBacktestingEngine(BacktestingForm);
public:
	BacktestingEngine engine;
private:
	Ui::Backtesting ui;
	QThread thread;
};
