#pragma once
#include <QObject>
#include "util.h"
#include "debug.h"

class StrategyBase:public QObject
{
	Q_OBJECT
public:
	virtual QString name() = 0;
	// 策略启动
	virtual void onStart() = 0;
	// 策略停止
	virtual void onStop() = 0;
	// 账户资金回报
	virtual void onAccount(TradingAccount) = 0;
	// 持仓回报
	virtual void onPositions(QVector<CThostFtdcInvestorPositionField>) = 0;
	// 委托回报
	virtual void onOrders(QVector<CThostFtdcOrderField>) = 0;
	// 行情回报
	virtual void onTick(QuoteField) = 0;
	// 打印日志接口
	void log(QString);
	// 买入开仓
	void buy(QString, double, int);
	// 卖出平仓
	void sell(QString, double, int);
	// 取消订单
	void cancel(TThostFtdcOrderSysIDType);
	/*
	// 卖出开仓
	void short(QString, double, int);
	// 买入平仓
	void cover(QString, double, int);
	*/
signals:
	void sendReqOrderInsert(CThostFtdcInputOrderField);
	void sendReqOrderAction(CThostFtdcInputOrderActionField);
};