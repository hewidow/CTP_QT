#include "StrategyDMAHistoryVolatility.h"

StrategyDMAHistoryVolatility::StrategyDMAHistoryVolatility()
{

}

StrategyDMAHistoryVolatility::~StrategyDMAHistoryVolatility()
{

}

QString StrategyDMAHistoryVolatility::name()
{
	return "多品种双均线策略（波动率）";
}

void StrategyDMAHistoryVolatility::onStart()
{
	period = 600;
	for (auto& i : instruments) {
		weights[i] = HV[i] = 0;
	}
}

void StrategyDMAHistoryVolatility::onStop()
{

}

void StrategyDMAHistoryVolatility::onPositions(QVector<CThostFtdcInvestorPositionField> t)
{

}
void StrategyDMAHistoryVolatility::onOrders(QVector<CThostFtdcOrderField> t)
{

}
void StrategyDMAHistoryVolatility::onTick(QuoteField tick)
{

}

void StrategyDMAHistoryVolatility::onKLine(KLine kLine)
{
	QString InstrumentID = kLine.InstrumentID;
	auto& q = kLineMap[InstrumentID];

	if (q.size() < period) return;

	auto getHV = [](QQueue<KLine>& que, int len) {
		QVector<double>xs;
		double sum = 0;
		for (int i = 0; i < len; ++i) {
			xs.push_back(std::log(que[i].closePrice / que[i + 1].closePrice));
			sum += xs.back();
		}
		double xb = sum / xs.size();
		double xe = 0;
		for (auto& x : xs) {
			xe += (x - xb) * (x - xb);
		}
		xe = sqrt(xe / xs.size());
		if (fabs(xe) <= 1e-15) return 0.0;
		return 1.0 / xe;
	};
	HV[InstrumentID] = getHV(q, period - 1);
	double sumHV = 0;
	for (auto& it : instruments) {
		sumHV += HV[it];
	}
	for (auto& it : instruments) {
		weights[it] = HV[it] / sumHV;
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
