#pragma once

#include "StrategyBase.h"

class StrategyDMAATR : public StrategyBase
{
public:
	StrategyDMAATR();
	~StrategyDMAATR();
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
	int TRPeriod = 20; // TR周期

	QMap<QString, double>ATR; // 均幅指标

	QMap<QString, QQueue<double> >TRMap; // TR队列
	QMap<QString, double >SumTR; // TR和
	double positiveSumATR = 0; // 正的ATR
};
