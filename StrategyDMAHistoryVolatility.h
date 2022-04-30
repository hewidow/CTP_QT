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
	int longPeriod = 60; // ����������
	int shortPeriod = 20; // �̾�������

	QMap<QString, double>HV; // ��ʷ������
};
