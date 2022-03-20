#pragma once

#include <QDialog>
#include <QCloseEvent>
#include <QDir>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include "ui_backtesting.h"
#include "util.h"
#include "debug.h"

class Backtesting : public QDialog
{
	Q_OBJECT

public:
	Backtesting(QWidget *parent = Q_NULLPTR);
	~Backtesting();
	// 错误弹窗
	void sendError(QString);
	// 显示可选数据库
	void showDatabase();
	// 初始化数据库
	bool initDatabase();
	// 加载数据
	void loadData();
	// 新建报单
	void receiveReqOrderInsert(CThostFtdcInputOrderField);
	// 修改报单
	void receiveReqOrderAction(CThostFtdcInputOrderActionField);
protected:
	// 重写关闭事件
	void closeEvent(QCloseEvent* event) override;
private slots:
	// 开始回测
	void on_start_clicked();
	// 暂停回测
	void on_stop_clicked();
signals:
	// 显示回测结果
	void sendBacktestingResult(BacktestingResult);
	// 发送回测开始还是暂停的状态
	void sendBacktestingStatus(bool);
    // 发送账户资金信息
    void sendTradingAccount(TradingAccount);
    // 发送投资者持仓
    void sendInvestorPositions(QVector<CThostFtdcInvestorPositionField>);
    // 发送报单信息
    void sendOrders(QVector<CThostFtdcOrderField>);
	// tick响应
	void sendRtnDepthMarketData(QuoteField);
	// 默认1分钟k线响应
	void sendKLine(KLine);
private:
	Ui::Backtesting ui;
	BacktestingResult result;
	QSqlDatabase db;
	TradingAccount account;
};
