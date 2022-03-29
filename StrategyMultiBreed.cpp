#include "StrategyMultiBreed.h"

StrategyMultiBreed::StrategyMultiBreed()
{

}

StrategyMultiBreed::~StrategyMultiBreed()
{

}

QString StrategyMultiBreed::name()
{
	return "多品种策略";
}

void StrategyMultiBreed::onStart()
{
	log("策略启动");
}

void StrategyMultiBreed::onStop()
{
	log("策略停止");
}

void StrategyMultiBreed::onPositions(QVector<CThostFtdcInvestorPositionField> t)
{
	positions = t;
	positionsMap.clear();
	for (auto& it : positions) {
		positionsMap[it.InstrumentID] = it;
	}
}
void StrategyMultiBreed::onOrders(QVector<CThostFtdcOrderField> t)
{
	orders = t;
	ordersMap.clear();
	for (auto& it : orders) {
		ordersMap[it.ExchangeID] = it;
	}
}
void StrategyMultiBreed::onTick(QuoteField tick)
{

}

void StrategyMultiBreed::onKLine(KLine kLine)
{
	auto& que = kLineMap[kLine.InstrumentID];
	que.push_front(kLine);
	while (que.size() > TICK_HISTORY_LENGTH) que.pop_back();

	QString InstrumentID = "IC2203";
	if (kLine.InstrumentID != InstrumentID) return;
	if (kLineMap.count(InstrumentID) == 0) return;
	auto& q = kLineMap[InstrumentID];
	auto getPriceTrend = [&](int len) -> int {
		if (q.size() <= len) return 0; // 无趋势
		bool up = true, down = true;
		for (int i = 0; i < len; ++i) {
			up &= (q[i].closePrice > q[i + 1].closePrice);
			down &= (q[i].closePrice < q[i + 1].closePrice);
		}
		if (up) return 1; // 上涨
		if (down) return -1; // 下跌
		return 0;
	};
	int trend = getPriceTrend(5);
	if (trend == 0) return;
	if (trend == 1) {
		buy(InstrumentID, q[0].closePrice, 1);
	}
	else {
		if (positionsMap.count(InstrumentID) != 0 && positionsMap[InstrumentID].Position > 0) {
			sell(InstrumentID, q[0].closePrice, positionsMap[InstrumentID].Position);
		}
	}
	while (que.size() > 0) que.pop_back();
}
