#pragma once

#include "StrategyBase.h"

class StrategyDMAHistoryVolatility : public StrategyBase
{
public:
	StrategyDMAHistoryVolatility();
	~StrategyDMAHistoryVolatility();
	QString name() override;
	void onStart() override;
	void onStop() override;
	void onPositions(QVector<CThostFtdcInvestorPositionField>) override;
	void onOrders(QVector<CThostFtdcOrderField>) override;
	void onTick(QuoteField) override;
	void onKLine(KLine) override;
private:
	int longPeriod = 60; // 长均线周期
	int shortPeriod = 20; // 短均线周期

	QMap<QString, double>HV; // 历史波动率
};
