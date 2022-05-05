#include "StrategyDMAATR.h"

StrategyDMAATR::StrategyDMAATR()
{

}

StrategyDMAATR::~StrategyDMAATR()
{

}

QString StrategyDMAATR::name()
{
	return "多品种双均线策略（ATR）";
}

void StrategyDMAATR::onStart()
{
	period = 600;
	ATR.clear();
	TRMap.clear();
	sumTR.clear();
	sumATR = 0;
	for (auto& i : instruments) {
		weights[i] = ATR[i] = sumTR[i] = 0;
	}
}

void StrategyDMAATR::onStop()
{

}

void StrategyDMAATR::onPositions(QVector<CThostFtdcInvestorPositionField> t)
{

}
void StrategyDMAATR::onOrders(QVector<CThostFtdcOrderField> t)
{

}
void StrategyDMAATR::onTick(QuoteField tick)
{

}

void StrategyDMAATR::onKLine(KLine kLine)
{
	QString InstrumentID = kLine.InstrumentID;
	auto& q = kLineMap[InstrumentID];
	auto& que = TRMap[InstrumentID];
	if (q.size() >= 2) {
		que.push_front(std::max({ fabs(q[0].highPrice - q[0].lowPrice),fabs(q[0].highPrice - q[1].closePrice),fabs(q[0].lowPrice - q[1].closePrice) }));
		sumTR[InstrumentID] += que.front();
	}
	while (que.size() > period - 1) sumTR[InstrumentID] -= que.back(), que.pop_back();

	if (q.size() < period) return;
	sumATR -= ATR[InstrumentID];
	if (fabs(sumTR[InstrumentID]) <= 1e-15) ATR[InstrumentID] = 0;
	else ATR[InstrumentID] = (period - 1) / sumTR[InstrumentID];
	sumATR += ATR[InstrumentID];
	if (sumATR <= 0) return;
	for (auto& it : instruments) {
		weights[it] = ATR[it] / sumATR;
	}
	auto shortMA = getMA(q, shortPeriod);
	auto longMA = getMA(q, longPeriod);
	// 短均线上穿长均线，做多（买）
	if (shortMA[0] >= longMA[0] && shortMA[1] < longMA[1]) {
		double money = tradingAccount.totalAssets * weights[InstrumentID] - positionsMap[InstrumentID].PositionCost;
		buy(InstrumentID, q[0].closePrice + 3, std::max(0, int(money / q[0].closePrice / 2)));
	}
	// 短均线下穿长均线，平多（卖）
	if (longMA[0] >= shortMA[0] && longMA[1] < shortMA[1]) {
		sell(InstrumentID, q[0].closePrice - 3, positionsMap[InstrumentID].OpenVolume - positionsMap[InstrumentID].CloseVolume);
	}
}
