#pragma once
#include <QThread>
#include <QTimer>
#include <QQueue>
#include <mutex>
#include "debug.h"
#include "util.h"

#define sDebug iDebug<<QString("【自动交易策略】")

class StrategyModel:public QThread
{
	Q_OBJECT
		
public:
	StrategyModel();
	~StrategyModel();
	void run() override;
	// 暂停子线程
	void pause();
	void buy(QString, double, int);
	void sell(QString, double, int);
	void cancel(TThostFtdcOrderSysIDType);
	void test();
public slots:
	 void timeOut();
	 void receiveRspLoginTd(CThostFtdcRspUserLoginField);
	 void receiveTradingAccount(TradingAccount);
	 void receiveInvestorPositions(QVector<CThostFtdcInvestorPositionField>);
	 void receiveOrders(QVector<CThostFtdcOrderField>);
	 void receiveAllInstruments(QVector<InstrumentField>);
	 void receiveRtnDepthMarketData(QuoteField);
signals:
	void sendReqOrderInsert(CThostFtdcInputOrderField);
	void sendReqOrderAction(CThostFtdcInputOrderActionField);
private:
	std::mutex mutex;
	QMap<QString, QQueue<QuoteField> >tickMap; // 历史行情
	CThostFtdcRspUserLoginField userInfo; // 用户信息
	TradingAccount tradingAccount; // 交易账户
	QVector<CThostFtdcInvestorPositionField> positions; // 持仓情况
	QMap<QString,CThostFtdcInvestorPositionField> positionsMap; // InstrumentID映射持仓
	QVector<CThostFtdcOrderField> orders; // 报单情况
	QMap<QString, CThostFtdcOrderField>ordersMap; // OrderSysID映射报单
	QVector<InstrumentField>instruments; // 合约信息
	QMap<QString, InstrumentField>instrumentsMap; // InstrumentID映射合约
};
