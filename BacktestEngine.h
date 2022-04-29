#pragma once

#include <algorithm>
#include <QObject>
#include <QThread>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include "util.h"
#include "debug.h"

class BacktestEngine : public QObject
{
	Q_OBJECT

public:
	explicit BacktestEngine(QObject* parent = nullptr);
	~BacktestEngine();
	// 清理持仓（比如移除零持仓的合约）
	void cleanPos();
	// 清理报单（比如移除已取消或已交易的报单）
	void cleanOrders();
	// 处理报单
	void solveOrders();
	// 查找对应合约在持仓中的下标
	int findPosIndex(const char*);
	// 计算统计结果
	void calcResult();
	// 计算期货均价
	double calcFuturesPrice();
signals:
	// 发送错误信息
	void sendError(QString);
	// 发送回测进度
	void sendBacktestProgress(int);
	// 发送回测结束
	void sendStopBacktestEngine(BacktestResult);
	// 发送账户资金信息
	void sendTradingAccount(TradingAccount);
	// 发送投资者持仓
	void sendInvestorPositions(QVector<CThostFtdcInvestorPositionField>);
	// 发送报单信息
	void sendOrders(QVector<CThostFtdcOrderField>);
	// 发送tick行情
	void sendRtnDepthMarketData(QuoteField);
	// 发送一分钟k线行情
	void sendKLine(KLine);
	// 发送合约盈亏情况
	void sendBacktestResultPos(QMap<QString, BacktestResultPos>);
	// 发送图表数据
	void sendBacktestChartData(BacktestChartData);
public slots:
	// 回测开始
	void receiveStartBacktestEngine(BacktestForm);
	// 新建报单
	void receiveReqOrderInsert(CThostFtdcInputOrderField);
	// 修改报单
	void receiveReqOrderAction(CThostFtdcInputOrderActionField);
	// 策略收到的k线数据
	void receiveReceivedKLine();
private:
	QSqlDatabase db; // 回测数据库
	BacktestForm form; // 回测初始参数表单
	BacktestResult result = { QDate(),QDate(),0 }; // 回测结果汇总
	QMap<QString, BacktestResultPos>resultPos; // 每个合约盈亏情况
	BacktestChartData chartData; // 图表数据汇总
	TradingAccount account; // 账户信息
	QMap<QString, BacktestInstrumentForm>instruments; // 回测数据里的全部合约信息
	QVector<KLine>kLines; // k线数据
	int receivedKLinesP = -2; // 当前待发送的k线数据下标
	QVector<CThostFtdcInvestorPositionField>pos; // 当前持仓
	QVector<CThostFtdcOrderField>orders; // 当前报单
	int orderSysID = 0; // 报单全局唯一标识
	bool firstRecord = true; // 记录首次期货均价
	bool startRecord = false; // 开始记录期货均价
	long long startTimeStamp = 0; // 开始记录时的毫秒时间戳
	double startFuturesPrice = 0; // 开始时的期货均价
};
