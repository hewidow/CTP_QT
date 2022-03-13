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
void StrategyModel::timeOut() {
    std::lock_guard<std::mutex>lock(mutex);
	sDebug << "正在运行";
    QString InstrumentID = "IC2203";
    if (tickMap.count(InstrumentID) == 0) return;
    auto& q = tickMap[InstrumentID];
    auto getPriceTrend = [&](int len) -> int {
        if (q.size() <= len) return 0; // 无趋势
        bool up = true, down = true;
        for (int i = 0; i < len; ++i) {
            up &= (q[i].LastPrice > q[i + 1].LastPrice);
            down &= (q[i].LastPrice < q[i + 1].LastPrice);
        }
        if (up) return 1; // 上涨
        if (down) return -1; // 下跌
        return 0;
    };
    int trend = getPriceTrend(5);
    if (trend==1) {
        buy(InstrumentID, q[0].LastPrice,1);
    }
    else if (trend == -1) {
        if (positionsMap.count(InstrumentID) != 0 && positionsMap[InstrumentID].Position > 0) {
            sell(InstrumentID, q[0].LastPrice, positionsMap[InstrumentID].Position);
        }
    }
}

void StrategyModel::pause()
{
	quit();
	wait();
}

void StrategyModel::buy(QString InstrumentID,double LastPrice,int VolumeTotalOriginal)
{
    if (instrumentsMap.count(InstrumentID) == 0) {
        sDebug << "下买单失败：合约不存在";
        return;
    }
    if (VolumeTotalOriginal <= 0) {
        sDebug << "下买单失败：数量小于等于0";
        return;
    }
    CThostFtdcInputOrderField ord = { 0 };
    strcpy_s(ord.ExchangeID, instrumentsMap[InstrumentID].ExchangeID.toStdString().c_str());
    strcpy_s(ord.InstrumentID, InstrumentID.toStdString().c_str());
    ord.LimitPrice = LastPrice;
    ord.VolumeTotalOriginal = VolumeTotalOriginal;
    ord.Direction = THOST_FTDC_D_Buy;
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
    sDebug << ord.InstrumentID << "买 价格" << ord.LimitPrice << "数量" << ord.VolumeTotalOriginal;
}
void StrategyModel::sell(QString InstrumentID, double LastPrice, int VolumeTotalOriginal)
{
    if (instrumentsMap.count(InstrumentID) == 0) {
        sDebug << "下卖单失败：合约不存在";
        return;
    }
    if (VolumeTotalOriginal <= 0) {
        sDebug << "下卖单失败：数量小于等于0";
        return;
    }
    CThostFtdcInputOrderField ord = { 0 };
    strcpy_s(ord.ExchangeID, instrumentsMap[InstrumentID].ExchangeID.toStdString().c_str());
    strcpy_s(ord.InstrumentID, InstrumentID.toStdString().c_str());
    ord.LimitPrice = LastPrice;
    ord.VolumeTotalOriginal = VolumeTotalOriginal;
    ord.Direction = THOST_FTDC_D_Sell;
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
    sDebug << ord.InstrumentID << "卖 价格" << ord.LimitPrice << "数量" << ord.VolumeTotalOriginal;
}
void StrategyModel::cancel(TThostFtdcOrderSysIDType OrderSysID)
{
    if (ordersMap.count(OrderSysID) == 0) {
        sDebug<<"取消报单失败：未找到该报单";
        return;
    }
    sDebug << "取消报单" <<"报单编号"<< OrderSysID;
    CThostFtdcInputOrderActionField a = { 0 };
    strcpy_s(a.OrderSysID ,OrderSysID);
    auto& o = ordersMap[OrderSysID];
    strcpy_s(a.ExchangeID, o.ExchangeID);
    strcpy_s(a.InstrumentID, o.InstrumentID);
    a.ActionFlag = THOST_FTDC_AF_Delete;

    emit sendReqOrderAction(a);
}
void StrategyModel::test()
{
	sDebug << "test";
}
void StrategyModel::receiveRtnDepthMarketData(QuoteField q)
{
    std::lock_guard<std::mutex>lock(mutex);
    auto& que = tickMap[q.InstrumentID];
    que.push_front(q);
    while (que.size()> TICK_HISTORY_LENGTH) que.pop_back();
}
void StrategyModel::receiveRspLoginTd(CThostFtdcRspUserLoginField u)
{
    std::lock_guard<std::mutex>lock(mutex);
    userInfo = u;
}
void StrategyModel::receiveTradingAccount(TradingAccount t)
{
    std::lock_guard<std::mutex>lock(mutex);
    tradingAccount = t;
}

void StrategyModel::receiveInvestorPositions(QVector<CThostFtdcInvestorPositionField> p)
{
    std::lock_guard<std::mutex>lock(mutex);
    positions = p;
    positionsMap.clear();
    for (auto& it : positions) {
        positionsMap[it.InstrumentID] = it;
    }
}
void StrategyModel::receiveOrders(QVector<CThostFtdcOrderField> o)
{
    std::lock_guard<std::mutex>lock(mutex);
    orders = o;
    ordersMap.clear();
    for (auto& it : orders) {
        ordersMap[it.ExchangeID] = it;
    }
}
void StrategyModel::receiveAllInstruments(QVector<InstrumentField>i)
{
    std::lock_guard<std::mutex>lock(mutex);
    instruments = i;
    instrumentsMap.clear();
    for (auto& it : instruments) {
        instrumentsMap[it.InstrumentID] = it;
    }
}