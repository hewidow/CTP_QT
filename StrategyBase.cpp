#include "StrategyBase.h"
StrategyBase::StrategyBase()
{
	
}
void StrategyBase::_onStart()
{
	period = 600;
	instruments = { "ag2203","IC2203","fu2203" };
	tradingAccount = TradingAccount{ 0 };
	kLineMap.clear();
	positions.clear();
	positionsMap.clear();
	orders.clear();
	ordersMap.clear();
	futuresPosWeightData.futuresPosWeightData.clear();
	startTimeStamp = 0;
	befTimeStamp = 0;
	weights.clear();
	onStart();
}
void StrategyBase::_onStop()
{
	onStop();
	emit sendFuturesPosWeightData(futuresPosWeightData);
}
void StrategyBase::_onPositions(QVector<CThostFtdcInvestorPositionField>p)
{
	positions = p;
	positionsMap.clear();
	for (auto& it : positions) {
		positionsMap[it.InstrumentID] = it;
	}
	onPositions(p);
}
void StrategyBase::_onOrders(QVector<CThostFtdcOrderField>o)
{
	orders = o;
	ordersMap.clear();
	for (auto& it : orders) {
		ordersMap[it.OrderSysID] = it;
	}
	onOrders(o);
}
void StrategyBase::_onKLine(KLine kLine)
{
	emit sendReceivedKLine();
	if (instruments.size() == 0 || instruments.contains(kLine.InstrumentID)) {
		recordPosWeight(kLine.dateTime.toMSecsSinceEpoch());
		auto& q = kLineMap[kLine.InstrumentID];
		q.push_front(kLine);
		while (q.size() > period) q.pop_back();
		onKLine(kLine);
	}
}
void StrategyBase::onAccount(TradingAccount t)
{
	tradingAccount = t;
}
void StrategyBase::log(QString s)
{
	if (backtest) return;
	iDebug << "【当前策略：" + name() + "】" << s;
}
void StrategyBase::buy(QString InstrumentID, double LastPrice, int VolumeTotalOriginal)
{
	if (VolumeTotalOriginal <= 0) return;
	CThostFtdcInputOrderField ord = { 0 };
	strcpy_s(ord.InstrumentID, InstrumentID.toStdString().c_str());
	ord.LimitPrice = LastPrice;
	ord.VolumeTotalOriginal = VolumeTotalOriginal;
	ord.Direction = THOST_FTDC_D_Buy;
	ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice; // 限价单
	ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open; // 开仓
	ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation; // 投机
	ord.TimeCondition = THOST_FTDC_TC_GTC; // 撤销前有效
	ord.VolumeCondition = THOST_FTDC_VC_AV; // 任意数量
	ord.MinVolume = 1; // 最小成交量
	ord.ContingentCondition = THOST_FTDC_CC_Immediately; // 触发条件
	ord.StopPrice = 0; // 止损价
	ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose; // 强平原因
	ord.IsAutoSuspend = 0; // 自动挂起标志
	sendReqOrderInsert(ord);
	log("买入开仓 | 代码 " + QString(ord.InstrumentID) + " | 价格 " + QString::number(ord.LimitPrice) + " | 数量 " + QString::number(ord.VolumeTotalOriginal));
}
void StrategyBase::sell(QString InstrumentID, double LastPrice, int VolumeTotalOriginal)
{
	if (VolumeTotalOriginal <= 0) return;
	CThostFtdcInputOrderField ord = { 0 };
	strcpy_s(ord.InstrumentID, InstrumentID.toStdString().c_str());
	ord.LimitPrice = LastPrice;
	ord.VolumeTotalOriginal = VolumeTotalOriginal;
	ord.Direction = THOST_FTDC_D_Buy;
	ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice; // 限价单
	ord.CombOffsetFlag[0] = THOST_FTDC_OF_Close; // 平仓
	ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation; // 投机
	ord.TimeCondition = THOST_FTDC_TC_GTC; // 撤销前有效
	ord.VolumeCondition = THOST_FTDC_VC_AV; // 任意数量
	ord.MinVolume = 1; // 最小成交量
	ord.ContingentCondition = THOST_FTDC_CC_Immediately; // 触发条件
	ord.StopPrice = 0; // 止损价
	ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose; // 强平原因
	ord.IsAutoSuspend = 0; // 自动挂起标志
	emit sendReqOrderInsert(ord);
	log("卖出平仓 | 代码 " + QString(ord.InstrumentID) + " | 价格 " + QString::number(ord.LimitPrice) + " | 数量 " + QString::number(ord.VolumeTotalOriginal));
}
void StrategyBase::cancel(TThostFtdcOrderSysIDType OrderSysID)
{
	log("取消报单 | 报单编号 " + QString(OrderSysID));
	CThostFtdcInputOrderActionField a = { 0 };
	strcpy_s(a.OrderSysID, OrderSysID);
	a.ActionFlag = THOST_FTDC_AF_Delete;

	emit sendReqOrderAction(a);
}

QVector<double> StrategyBase::getMA(QQueue<KLine>&q,int len) {
	QVector<double>MA;
	if (q.size() <= len) {
		iDebug << "getMA error: the queue is not long enough";
		return QVector<double>{0,0};
	}
	double sum = 0;
	for (int i = 0; i <= len; ++i) {
		sum += q[i].closePrice;
		if (i >= len) sum -= q[i - len].closePrice;
		if (i >= len - 1) MA.push_back(sum / len);
	}
	return MA;
};
void StrategyBase::recordPosWeight(long long nowTimeStamp) {
	if (nowTimeStamp != befTimeStamp && nowTimeStamp - startTimeStamp >= DOT_INTERVAL) {
		startTimeStamp = nowTimeStamp;
		for (auto it = weights.begin(); it != weights.end(); ++it) {
			futuresPosWeightData.futuresPosWeightData[it.key()].push_back({ startTimeStamp, Util::formatDoubleTwoDecimal(it.value()) });
		}
	}
	befTimeStamp = nowTimeStamp;
}