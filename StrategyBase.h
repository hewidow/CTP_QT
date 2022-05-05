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
	// 查询持仓回调
	virtual void onPositions(QVector<CThostFtdcInvestorPositionField>) = 0;
	// 查询委托回调
	virtual void onOrders(QVector<CThostFtdcOrderField>) = 0;
	// 接收Tick数据
	virtual void onTick(QuoteField) = 0;
	// 接收K线数据
	virtual void onKLine(KLine) = 0;
	// 内部函数
	void _onStart();
	// 内部函数
	void _onStop();
	// 内部函数
	void _onPositions(QVector<CThostFtdcInvestorPositionField>);
	// 内部函数
	void _onOrders(QVector<CThostFtdcOrderField>);
	// 内部函数
	void _onKLine(KLine);
	// 查询账户资金回调
	void onAccount(TradingAccount);
	// 打印日志接口
	void log(QString);
	// 做多
	void buy(QString, double, int);
	// 平多
	void sell(QString, double, int);
	// 取消报单
	void cancel(TThostFtdcOrderSysIDType);
	/*
	// 做空
	void short(QString, double, int);
	// 平空
	void cover(QString, double, int);
	*/
	// 获取均线
	QVector<double> getMA(QQueue<KLine>&, int);
	// 记录持仓权重信息
	void recordPosWeight(long long);
signals:
	// 新建报单
	void sendReqOrderInsert(CThostFtdcInputOrderField);
	// 操作报单
	void sendReqOrderAction(CThostFtdcInputOrderActionField);
	// 告诉回测系统收到K线的信号
	void sendReceivedKLine();
	// 发送记录的持仓权重数据给折线图
	void sendFuturesPosWeightData(FuturesPosWeightData);
public:
	bool backtest = false; // 是否处于回测
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