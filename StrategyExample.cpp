#include "StrategyExample.h"

StrategyExample::StrategyExample()
{

}

StrategyExample::~StrategyExample()
{

}

QString StrategyExample::name()
{
	return "示例策略";
}

void StrategyExample::onStart()
{
	log("策略启动");
}

void StrategyExample::onStop()
{
	log("策略停止");
}

void StrategyExample::onPositions(QVector<CThostFtdcInvestorPositionField> t)
{
	positions = t;
	positionsMap.clear();
	for (auto& it : positions) {
		positionsMap[it.InstrumentID] = it;
	}
}
void StrategyExample::onOrders(QVector<CThostFtdcOrderField> t)
{
	orders = t;
	ordersMap.clear();
	for (auto& it : orders) {
		ordersMap[it.ExchangeID] = it;
	}
}
void StrategyExample::onTick(QuoteField tick)
{
	auto& que = tickMap[tick.InstrumentID];
	que.push_front(tick);
	while (que.size() > TICK_HISTORY_LENGTH) que.pop_back();

	QString InstrumentID = "IC2203";
	if (tick.InstrumentID != InstrumentID) return;
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
	if (trend == 0) return;
	if (trend == 1) {
		buy(InstrumentID, q[0].LastPrice, 1);
	}
	else {
		if (positionsMap.count(InstrumentID) != 0 && positionsMap[InstrumentID].Position > 0) {
			sell(InstrumentID, q[0].LastPrice, positionsMap[InstrumentID].Position);
		}
	}
	while (que.size() > 0) que.pop_back();
}

void StrategyExample::onKLine(KLine)
{
}
