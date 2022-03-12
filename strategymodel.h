#pragma once
#include <QThread>
#include <QTimer>
#include <deque>
#include <mutex>
#include "global.h"
#include "debug.h"

class StrategyModel:public QThread
{
	Q_OBJECT
		
public:
	StrategyModel();
	~StrategyModel();
	void run() override;
	void play(CThostFtdcRspUserLoginField);
	// 暂停子线程
	void pause();
	void buy(QuoteField);
	void sell(QuoteField);
	void cancel(QuoteField);
	// 比较行情价格，前者高为true
	bool compareHistory(int, int);
	void test();
public slots:
	 void timeOut();
	 void receiveRtnDepthMarketData(QuoteField);
signals:
	void sendReqOrderInsert(CThostFtdcInputOrderField);
	void sendReqOrderAction(CThostFtdcInputOrderActionField);
private:
	CThostFtdcRspUserLoginField userInfo;
	std::mutex queueMutex; // 队列锁
	// 历史行情
	std::deque<QuoteField>history;
	// 当前持仓
	int nowVolume = 0;
};
