#pragma once

#include <QDialog>
#include <QCloseEvent>
#include <QDir>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QThread>
#include <QWebEngineView>
#include "ui_backtest.h"
#include "util.h"
#include "debug.h"
#include "BacktestEngine.h"

class Backtest : public QDialog
{
	Q_OBJECT

public:
	Backtest(QWidget* parent = Q_NULLPTR);
	~Backtest();
	// 显示可选数据库
	void showDatabase();
protected:
	// 重写关闭事件
	void closeEvent(QCloseEvent* event) override;
private slots:
	// 显示错误弹窗
	void receiveError(QString);
	// 显示回测进度
	void receiveBacktestProgress(int);
	// 回测结束
	void receiveStopBacktestEngine(BacktestResult);
	// 开始回测按钮
	void on_start_clicked();
	// 暂停回测按钮
	void on_stop_clicked();
signals:
	// 发送回测开始还是暂停的状态
	void sendBacktestStatus(bool);
	// 显示回测结果
	void sendBacktestResult(BacktestResult);
	// 回测开始
	void sendStartBacktestEngine(BacktestForm);
private:
	Ui::Backtest ui;
	BacktestEngine* engine;
	QThread* thread;
	QVector<int> floatingProfitLossSeries; // 浮动盈亏数据
	/* 以下为转发 */
signals:
	void sendTradingAccount(TradingAccount);
	void sendInvestorPositions(QVector<CThostFtdcInvestorPositionField>);
	void sendOrders(QVector<CThostFtdcOrderField>);
	void sendRtnDepthMarketData(QuoteField);
	void sendKLine(KLine);
	void sendReqOrderInsert(CThostFtdcInputOrderField);
	void sendReqOrderAction(CThostFtdcInputOrderActionField);
};
