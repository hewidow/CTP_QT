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

}

void StrategyExample::onKLine(KLine kLine)
{
	auto& que = kLineMap[kLine.InstrumentID];
	que.push_front(kLine);
	while (que.size() > TICK_HISTORY_LENGTH) que.pop_back();

	QString InstrumentID = kLine.InstrumentID;
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
		buy(InstrumentID, q[0].closePrice, std::max(1, int(tradingAccount.Available / q[0].closePrice / 2)));
	}
	else {
		if (positionsMap.count(InstrumentID) != 0 && positionsMap[InstrumentID].OpenVolume - positionsMap[InstrumentID].CloseVolume > 0) {
			sell(InstrumentID, q[0].closePrice, std::max(1, (positionsMap[InstrumentID].OpenVolume - positionsMap[InstrumentID].CloseVolume) / 2));
		}
	}
	while (que.size() > 0) que.pop_back();
}