#pragma once
#include <QThread>
#include <QTimer>
#include <QQueue>
#include <mutex>
#include "debug.h"
#include "util.h"
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
	void cancel(QuoteField);
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
	QMap<QString,CThostFtdcInvestorPositionField> positionMap; // 持仓情况映射
	QVector<CThostFtdcOrderField> orders; // 报单情况
	QVector<InstrumentField>instruments; // 合约信息
	QMap<QString, InstrumentField>instrumentMap; // 合约映射
};
