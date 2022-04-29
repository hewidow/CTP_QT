#include "StrategyExample.h"

StrategyExample::StrategyExample()
{

}

StrategyExample::~StrategyExample()
{

}

QString StrategyExample::name()
{
	return "多品种示例策略";
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
		ordersMap[it.OrderSysID] = it;
	}
}
void StrategyExample::onTick(QuoteField tick)
{

}

void StrategyExample::onKLine(KLine kLine)
{
	QString InstrumentID = kLine.InstrumentID;

	auto& q = kLineMap[InstrumentID];
	q.push_front(kLine);
	while (q.size() > TICK_HISTORY_LENGTH) q.pop_back();

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
		// 连续5个上涨信号，每次买入可用资金的一半
		buy(InstrumentID, q[0].closePrice + 3, std::max(1, int(tradingAccount.Available / q[0].closePrice / 2)));
	}
	else {
		if (positionsMap.count(InstrumentID) && positionsMap[InstrumentID].OpenVolume - positionsMap[InstrumentID].CloseVolume > 0) {
			// 连续5个下跌信号，每次卖出持仓的一半
			sell(InstrumentID, q[0].closePrice - 3, std::max(1, (positionsMap[InstrumentID].OpenVolume - positionsMap[InstrumentID].CloseVolume) / 2));
		}
	}
	// 清空上涨信号
	while (q.size() > 0) q.pop_back();
}