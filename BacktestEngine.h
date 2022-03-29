#pragma once

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
	// 清理订单
	void cleanOrders();
	// 处理订单
	void solveOrders();
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
	TradingAccount account;
	QMap<QString, InstrumentForm>instruments;
	QVector<KLine>kLines;
	int kLinesP = 0;
	QVector<CThostFtdcInvestorPositionField>pos;
	QVector<CThostFtdcOrderField>orders;
	int orderSysID = 0;
};
