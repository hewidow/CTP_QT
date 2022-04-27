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

}
