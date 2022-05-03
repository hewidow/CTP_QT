#include "StrategyDMAEqualWeight.h"

StrategyDMAEqualWeight::StrategyDMAEqualWeight()
{

}

StrategyDMAEqualWeight::~StrategyDMAEqualWeight()
{

}

QString StrategyDMAEqualWeight::name()
{
	return "多品种双均线策略（等权）";
}

void StrategyDMAEqualWeight::onStart()
{
	period = 61;
	for (auto& i : instruments) {
		weights[i] = 1.0 / instruments.size();
	}
}

void StrategyDMAEqualWeight::onStop()
{

}

void StrategyDMAEqualWeight::onPositions(QVector<CThostFtdcInvestorPositionField> t)
{

}
void StrategyDMAEqualWeight::onOrders(QVector<CThostFtdcOrderField> t)
{

}
void StrategyDMAEqualWeight::onTick(QuoteField tick)
{

}

void StrategyDMAEqualWeight::onKLine(KLine kLine)
{
	auto& q = kLineMap[kLine.InstrumentID];

	if (q.size() < period) return;
	auto shortMA = getMA(q, shortPeriod);
	auto longMA = getMA(q, longPeriod);
	// 短均线上穿长均线，做多（买）
	if (shortMA[0] >= longMA[0] && shortMA[1] < longMA[1]) {
		double money = tradingAccount.totalAssets * weights[kLine.InstrumentID] - positionsMap[kLine.InstrumentID].PositionCost;
		buy(kLine.InstrumentID, q[0].closePrice + 3, std::max(0, int(money / q[0].closePrice / 2)));
	}
	// 短均线下穿长均线，平多（卖）
	if (longMA[0] >= shortMA[0] && longMA[1] < shortMA[1]) {
		sell(kLine.InstrumentID, q[0].closePrice - 3, positionsMap[kLine.InstrumentID].OpenVolume - positionsMap[kLine.InstrumentID].CloseVolume);
	}
}
