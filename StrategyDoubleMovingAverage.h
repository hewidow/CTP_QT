#pragma once

#include <QQueue>
#include "StrategyBase.h"

class StrategyDoubleMovingAverage : public StrategyBase
{
public:
	StrategyDoubleMovingAverage();
	~StrategyDoubleMovingAverage();
	QString name() override;
	void onStart() override;
	void onStop() override;
	void onPositions(QVector<CThostFtdcInvestorPositionField>) override;
	void onOrders(QVector<CThostFtdcOrderField>) override;
	void onTick(QuoteField) override;
	void onKLine(KLine) override;
private:
	QMap<QString, QQueue<KLine> >kLineMap; // 历史行情
	QVector<CThostFtdcInvestorPositionField> positions; // 持仓情况
	QMap<QString, CThostFtdcInvestorPositionField> positionsMap; // InstrumentID映射持仓
	QVector<CThostFtdcOrderField> orders; // 报单情况
	QMap<QString, CThostFtdcOrderField>ordersMap; // OrderSysID映射报单

	int period = 61; // 历史数据保留长度
	int longPeriod = 60; // 长均线周期
	int shortPeriod = 20; // 短均线周期
};
