#include "StrategyDMAHistoryVolatility.h"

StrategyDMAHistoryVolatility::StrategyDMAHistoryVolatility()
{
	for (auto& i : instruments) {
		weights[i] = HV[i] = 0;
	}
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
	log("策略启动");
}

void StrategyDMAHistoryVolatility::onStop()
{
	log("策略停止");
}

void StrategyDMAHistoryVolatility::onPositions(QVector<CThostFtdcInvestorPositionField> t)
{
	positions = t;
	positionsMap.clear();
	for (auto& it : positions) {
		positionsMap[it.InstrumentID] = it;
	}
}
void StrategyDMAHistoryVolatility::onOrders(QVector<CThostFtdcOrderField> t)
{
	orders = t;
	ordersMap.clear();
	for (auto& it : orders) {
		ordersMap[it.OrderSysID] = it;
	}
}
void StrategyDMAHistoryVolatility::onTick(QuoteField tick)
{

}

void StrategyDMAHistoryVolatility::onKLine(KLine kLine)
{
	if (instruments.contains(kLine.InstrumentID)) {
		long long nowTimeStamp = kLine.dateTime.toMSecsSinceEpoch();
		if (nowTimeStamp != befTimeStamp && nowTimeStamp - startTimeStamp >= DOT_INTERVAL) {
			startTimeStamp = nowTimeStamp;
			for (auto it = weights.begin(); it != weights.end(); ++it) {
				futuresPosWeightData.futuresPosWeightData[it.key()].push_back({ startTimeStamp, Util::formatDoubleTwoDecimal(it.value()) });
			}
		}
		befTimeStamp = nowTimeStamp;

		auto& q = kLineMap[kLine.InstrumentID];
		q.push_front(kLine);
		while (q.size() > period) q.pop_back();

		if (q.size() < period) return;

		auto getHV = [](QQueue<KLine>& que, int len) {
			QVector<double>xs;
			double sum = 0;
			for (int i = len - 1; i < que.size(); ++i) {
				xs.push_back(std::log(que[i - len + 1].closePrice / que[i].closePrice));
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
		HV[kLine.InstrumentID] = getHV(q, shortPeriod);
		double sumHV = 0;
		for (auto& it : instruments) {
			sumHV += HV[it];
		}
		for (auto& it : instruments) {
			weights[it] = HV[it] / sumHV;
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
