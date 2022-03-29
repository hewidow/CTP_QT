#pragma once

#include <QQueue>
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
	QMap<QString, QQueue<QuoteField> >tickMap; // 历史行情
	QVector<CThostFtdcInvestorPositionField> positions; // 持仓情况
	QMap<QString, CThostFtdcInvestorPositionField> positionsMap; // InstrumentID映射持仓
	QVector<CThostFtdcOrderField> orders; // 报单情况
	QMap<QString, CThostFtdcOrderField>ordersMap; // OrderSysID映射报单
};
