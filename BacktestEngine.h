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
	// 发送行情数据
	void sendData();
	// 发送图表数据
	void sendBacktestChartData(BacktestChartData);
public slots:
	// 回测开始
	void receiveStartBacktestEngine(BacktestForm);
	// 新建报单
	void receiveReqOrderInsert(CThostFtdcInputOrderField);
	// 修改报单
	void receiveReqOrderAction(CThostFtdcInputOrderActionField);
	// 接收行情数据
	void receiveData();
private:
	struct InstrumentForm {
		QString ExchangeID;
		QString InstrumentID;
		double Price;
	};
	QSqlDatabase db;
	BacktestForm form;
	BacktestResult result = { QDate(),QDate(),0 };
	BacktestChartData chartData;
	TradingAccount account;
	QMap<QString, InstrumentForm>instruments;
	QVector<KLine>kLines;
	int kLinesP = 0;
	QVector<CThostFtdcInvestorPositionField>pos;
	QVector<CThostFtdcOrderField>orders;
	int orderSysID = 0;
	double startFuturesPrice;
	bool firstRecord = true; // 记录首次期货均价
	bool startRecord = false; // 开始记录期货均价
	long long startTimeStamp = 0;
};
