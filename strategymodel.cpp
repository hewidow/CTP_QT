#include "strategymodel.h"

StrategyModel::StrategyModel()
{

}

StrategyModel::~StrategyModel()
{
	pause();
}

void StrategyModel::run()
{
	QTimer timer;
	connect(&timer, &QTimer::timeout, this, &StrategyModel::timeOut);
	timer.start(STRATEGY_MODEL_RUN_INTERVAL);
	exec();
}

void StrategyModel::play(CThostFtdcRspUserLoginField u)
{
	userInfo = u;
	start();
}

void StrategyModel::timeOut() {
    std::lock_guard<std::mutex>lock(queueMutex);
	iDebug << "strategy running";
    if (history.size() >= 3) {
        if (compareHistory(0, 1) && compareHistory(1, 2)) buy(history.front());
        else if (!compareHistory(0, 1) && !compareHistory(1, 2)) sell(history.front());
    }
}

void StrategyModel::pause()
{
	quit();
	wait();
}

void StrategyModel::buy(QuoteField q)
{
    iDebug << "买" << "当前持仓：" << nowVolume;
    CThostFtdcInputOrderField ord = { 0 };

    strcpy_s(ord.ExchangeID, q.ExchangeID.toStdString().c_str());
    strcpy_s(ord.InstrumentID, q.InstrumentID.toStdString().c_str());
    ord.Direction = THOST_FTDC_D_Buy;
    ord.LimitPrice = q.LastPrice+10;
    ord.VolumeTotalOriginal = 1;

    nowVolume += 1;

    strcpy_s(ord.BrokerID, userInfo.BrokerID);
    strcpy_s(ord.InvestorID, userInfo.UserID);
    strcpy_s(ord.UserID, userInfo.UserID);
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice; // 限价单
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open; // 开仓

    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation; // 投机
    ord.TimeCondition = THOST_FTDC_TC_GFD; // 当日有效
    ord.VolumeCondition = THOST_FTDC_VC_AV; //任意数量
    ord.MinVolume = 1; // 最小成交量
    ord.ContingentCondition = THOST_FTDC_CC_Immediately; // 触发条件
    ord.StopPrice = 0; // 止损价
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose; // 强平原因
    ord.IsAutoSuspend = 0; // 自动挂起标志
	sendReqOrderInsert(ord);
    
}

void StrategyModel::sell(QuoteField q)
{
    iDebug << "卖"<<"当前持仓："<<nowVolume;
    if (nowVolume == 0) return;
    CThostFtdcInputOrderField ord = { 0 };

    strcpy_s(ord.ExchangeID, q.ExchangeID.toStdString().c_str());
    strcpy_s(ord.InstrumentID, q.InstrumentID.toStdString().c_str());
    ord.Direction = THOST_FTDC_D_Buy;
    ord.LimitPrice = q.LastPrice - 10;
    ord.VolumeTotalOriginal = nowVolume;

    nowVolume = 0;

    strcpy_s(ord.BrokerID, userInfo.BrokerID);
    strcpy_s(ord.InvestorID, userInfo.UserID);
    strcpy_s(ord.UserID, userInfo.UserID);
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice; // 限价单
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open; // 开仓

    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation; // 投机
    ord.TimeCondition = THOST_FTDC_TC_GFD; // 当日有效
    ord.VolumeCondition = THOST_FTDC_VC_AV; //任意数量
    ord.MinVolume = 1; // 最小成交量
    ord.ContingentCondition = THOST_FTDC_CC_Immediately; // 触发条件
    ord.StopPrice = 0; // 止损价
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose; // 强平原因
    ord.IsAutoSuspend = 0; // 自动挂起标志
	sendReqOrderInsert(ord);
}

void StrategyModel::cancel(QuoteField)
{
}

void StrategyModel::test()
{
	iDebug << "test";
}
bool StrategyModel::compareHistory(int i, int j) {
	return history[i].LastPrice > history[j].LastPrice;
}
void StrategyModel::receiveRtnDepthMarketData(QuoteField q)
{
	if (q.InstrumentID != "IC2203") return;
    std::lock_guard<std::mutex>lock(queueMutex);
	history.push_front(q);
    while (history.size()>100) history.pop_back();
}
