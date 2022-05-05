#pragma once

#include "StrategyBase.h"

class StrategyExample : public StrategyBase
{
public:
	StrategyExample();
	~StrategyExample();
	QString name() override;
	void onStart() override;
	void onStop() override;
	void onPositions(QVector<CThostFtdcInvestorPositionField>) override;
	void onOrders(QVector<CThostFtdcOrderField>) override;
	void onTick(QuoteField) override;
	void onKLine(KLine) override;
private:
	int signalLimit = 3;
};
