#pragma once

#include <QDialog>
#include <QCloseEvent>
#include "ui_backtesting.h"
#include "util.h"
#include "debug.h"

class Backtesting : public QDialog
{
	Q_OBJECT

public:
	Backtesting(QWidget *parent = Q_NULLPTR);
	~Backtesting();
	// 新建报单
	void receiveReqOrderInsert(CThostFtdcInputOrderField);
	// 修改报单
	void receiveReqOrderAction(CThostFtdcInputOrderActionField);
	// 加载数据
	void loadData();
protected:
	// 重写关闭事件
	void closeEvent(QCloseEvent* event) override;
private slots:
	// 开始回测
	void on_start_clicked();
	// 暂停回测
	void on_stop_clicked();
signals:
	// 发送回测开始还是暂停的状态
	void sendBacktestingStatus(bool);
    // 发送账户资金信息
    void sendTradingAccount(TradingAccount);
    // 发送投资者持仓
    void sendInvestorPositions(QVector<CThostFtdcInvestorPositionField>);
    // 发送报单信息
    void sendOrders(QVector<CThostFtdcOrderField>);
	// 订阅合约响应
	void sendRtnDepthMarketData(QuoteField);



private:
	Ui::Backtesting ui;
};
