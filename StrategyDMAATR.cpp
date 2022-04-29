#include "StrategyDMAATR.h"

StrategyDMAATR::StrategyDMAATR()
{
	for (auto& i : instruments) {
		weights[i] = ATR[i] = 0;
	}
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
	log("策略启动");
}

void StrategyDMAATR::onStop()
{
	log("策略停止");
}

void StrategyDMAATR::onPositions(QVector<CThostFtdcInvestorPositionField> t)
{
	positions = t;
	positionsMap.clear();
	for (auto& it : positions) {
		positionsMap[it.InstrumentID] = it;
	}
}
void StrategyDMAATR::onOrders(QVector<CThostFtdcOrderField> t)
{
	orders = t;
	ordersMap.clear();
	for (auto& it : orders) {
		ordersMap[it.OrderSysID] = it;
	}
}
void StrategyDMAATR::onTick(QuoteField tick)
{

}

void StrategyDMAATR::onKLine(KLine kLine)
{
	if (instruments.contains(kLine.InstrumentID)) {
		auto& q = kLineMap[kLine.InstrumentID];
		q.push_front(kLine);
		while (q.size() > period) q.pop_back();

		if (q.size() < period) return;

		auto getATR = [](QQueue<KLine>& que, int len) {
			QVector<double>xs;
			double sum = 0;
			for (int i = len - 1; i < que.size(); ++i) {
				xs.push_back(std::max(que[i - len + 1].highPrice, que[i].closePrice) - std::min(que[i - len + 1].lowPrice, que[i].closePrice));
				sum += xs.back();
			}
			return sum / xs.size();
		};
		ATR[kLine.InstrumentID] = getATR(q, shortPeriod);
		double sumATR = 0;
		for (auto& it : instruments) {
			if (ATR[it] > 0) sumATR += ATR[it];
		}
		if (sumATR <= 0) return;
		for (auto& it : instruments) {
			weights[it] = std::max(0.0, ATR[it]) / sumATR;
		}
		auto getMA = [&](int len) {
			QVector<double>MA;
			double sum = 0;
			for (int i = 0; i < q.size(); ++i) {
				sum += q[i].closePrice;
				if (i >= len) sum -= q[i - len].closePrice;
				if (i >= len - 1) MA.push_back(sum / len);
			}
			return MA;
		};
		auto shortMA = getMA(shortPeriod);
		auto longMA = getMA(longPeriod);
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
}
