#include "StrategyExample.h"

StrategyExample::StrategyExample() {}
StrategyExample::~StrategyExample() {}
QString StrategyExample::name() { return "多品种简单Tick策略"; }
void StrategyExample::onStart() {}
void StrategyExample::onStop() {}
void StrategyExample::onPositions(QVector<CThostFtdcInvestorPositionField> t) {}
void StrategyExample::onOrders(QVector<CThostFtdcOrderField> t) {}
void StrategyExample::onTick(QuoteField tick) {}
void StrategyExample::onKLine(KLine kLine)
{
	auto& q = kLineMap[kLine.InstrumentID];

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
	int trend = getPriceTrend(signalLimit);
	if (trend == 0) return;
	if (trend == 1) {
		// 连续signalLimit个上涨信号，每次买入可用资金的一半
		buy(kLine.InstrumentID, q[0].closePrice + 3, std::max(1, int(tradingAccount.Available / q[0].closePrice / 2)));
	}
	else {
		if (positionsMap.count(kLine.InstrumentID) && positionsMap[kLine.InstrumentID].OpenVolume - positionsMap[kLine.InstrumentID].CloseVolume > 0) {
			// 连续signalLimit个下跌信号，卖出该合约全部持仓
			sell(kLine.InstrumentID, q[0].closePrice - 3, positionsMap[kLine.InstrumentID].OpenVolume - positionsMap[kLine.InstrumentID].CloseVolume);
		}
	}
	// 清空历史数据
	q.clear();
}