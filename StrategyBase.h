#pragma once
#include <QObject>
#include "util.h"
#include "debug.h"

class StrategyBase :public QObject
{
	Q_OBJECT
public:
	StrategyBase();
	// 策略名称
	virtual QString name() = 0;
	// 策略启动
	virtual void onStart() = 0;
	// 策略停止
	virtual void onStop() = 0;
	// 持仓回报
	virtual void onPositions(QVector<CThostFtdcInvestorPositionField>) = 0;
	// 委托回报
	virtual void onOrders(QVector<CThostFtdcOrderField>) = 0;
	// tick回报
	virtual void onTick(QuoteField) = 0;
	// k线回报
	virtual void onKLine(KLine) = 0;
	// 启动内部函数
	void _onStart();
	// 停止内部函数
	void _onStop();
	// k线回报内部函数
	void _onKLine(KLine);
	// 账户资金回报
	void onAccount(TradingAccount);
	// 打印日志接口
	void log(QString);
	// 买入开仓
	void buy(QString, double, int);
	// 买入平仓
	void sell(QString, double, int);
	// 取消订单
	void cancel(TThostFtdcOrderSysIDType);
	/*
	// 卖出开仓
	void short(QString, double, int);
	// 卖出平仓
	void cover(QString, double, int);
	*/
signals:
	void sendReqOrderInsert(CThostFtdcInputOrderField);
	void sendReqOrderAction(CThostFtdcInputOrderActionField);
	void sendReceivedKLine();
	void sendFuturesPosWeightData(FuturesPosWeightData);
public:
	bool backtest = false;
protected:
	TradingAccount tradingAccount; // 交易账户资金信息
	FuturesPosWeightData futuresPosWeightData; // 持仓权重数据
	QVector<QString>instruments{ "ag2203","IC2203","fu2203" }; // 需要交易的合约品种名称
	QMap<QString, double>weights; // 可使用资金的权重
	long long startTimeStamp = 0; // 之前记录数据时的时间戳
	long long befTimeStamp = 0; // 上一个时间戳
};