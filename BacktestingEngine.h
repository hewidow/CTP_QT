#pragma once

#include <QObject>
#include <QVector>
#include <QSqlQuery>
#include "util.h"
#include "debug.h"

class BacktestingEngine : public QObject
{
	Q_OBJECT

public:
	explicit BacktestingEngine(QObject* parent = nullptr);
	~BacktestingEngine();
signals:
	// 发送错误信息
	void sendError(QString);
	// 回测结束
	void endBacktestingEngine(BacktestingResult);
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
public slots:
	// 回测开始
	void startBacktestingEngine(BacktestingForm);
	// 新建报单
	void receiveReqOrderInsert(CThostFtdcInputOrderField);
	// 修改报单
	void receiveReqOrderAction(CThostFtdcInputOrderActionField);
private:
	BacktestingForm form;
	BacktestingResult result;
	TradingAccount account;
	QVector<KLine>kLines;
	QSqlDatabase db;
};
