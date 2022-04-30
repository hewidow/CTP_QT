#include "StrategyDoubleMovingAverage.h"

StrategyDoubleMovingAverage::StrategyDoubleMovingAverage()
{

}

StrategyDoubleMovingAverage::~StrategyDoubleMovingAverage()
{

}

QString StrategyDoubleMovingAverage::name()
{
	return "多品种双均线策略（随机）";
}

void StrategyDoubleMovingAverage::onStart()
{
	log("策略启动");
	period = 61;
}

void StrategyDoubleMovingAverage::onStop()
{
	log("策略停止");
}

void StrategyDoubleMovingAverage::onPositions(QVector<CThostFtdcInvestorPositionField> t)
{

}
void StrategyDoubleMovingAverage::onOrders(QVector<CThostFtdcOrderField> t)
{

}
void StrategyDoubleMovingAverage::onTick(QuoteField tick)
{

}

void StrategyDoubleMovingAverage::onKLine(KLine kLine)
{
	auto& q = kLineMap[kLine.InstrumentID];

	if (q.size() < period) return;

	auto shortMA = getMA(q, shortPeriod);
	auto longMA = getMA(q, longPeriod);
	// 短均线上穿长均线，做多（买）
	if (shortMA[0] >= longMA[0] && shortMA[1] < longMA[1]) {
		buy(kLine.InstrumentID, q[0].closePrice + 3, std::max(1, int(tradingAccount.Available / q[0].closePrice / 2)));
	}
	// 短均线下穿长均线，平多（卖）
	if (longMA[0] >= shortMA[0] && longMA[1] < shortMA[1]) {
		sell(kLine.InstrumentID, q[0].closePrice - 3, positionsMap[kLine.InstrumentID].OpenVolume - positionsMap[kLine.InstrumentID].CloseVolume);
	}
}
