#pragma once
#include <QObject>
#include <QQueue>
#include <algorithm>
#include <cmath>
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
	// 持仓回报内部函数
	void _onPositions(QVector<CThostFtdcInvestorPositionField>);
	// 委托回报内部函数
	void _onOrders(QVector<CThostFtdcOrderField>);
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
	QVector<double> getMA(QQueue<KLine>&, int);
	void recordPosWeight(long long);
signals:
	void sendReqOrderInsert(CThostFtdcInputOrderField);
	void sendReqOrderAction(CThostFtdcInputOrderActionField);
	void sendReceivedKLine();
	void sendFuturesPosWeightData(FuturesPosWeightData);
public:
	bool backtest = false;
protected:
	QVector<QString>instruments; // 需要交易的合约品种名称
	int period; // 历史数据保留长度
	TradingAccount tradingAccount; // 交易账户资金信息
	QMap<QString, QQueue<KLine> >kLineMap; // 历史行情
	QVector<CThostFtdcInvestorPositionField> positions; // 持仓情况
	QMap<QString, CThostFtdcInvestorPositionField> positionsMap; // InstrumentID映射持仓
	QVector<CThostFtdcOrderField> orders; // 报单情况
	QMap<QString, CThostFtdcOrderField>ordersMap; // OrderSysID映射报单
	FuturesPosWeightData futuresPosWeightData; // 持仓权重数据	
	QMap<QString, double>weights; // 可使用资金的权重
	long long startTimeStamp; // 之前记录数据时的时间戳
	long long befTimeStamp; // 上一个时间戳
};