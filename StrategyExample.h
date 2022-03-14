#pragma once
#include <QQueue>
#include "StrategyBase.h"


class StrategyExample: public StrategyBase
{
public:
	StrategyExample();
	~StrategyExample();
	QString name() override;
	// 策略启动
	void onStart() override;
	// 策略停止
	void onStop() override;
	// 账户资金回报
	void onAccount(TradingAccount) override;
	// 持仓回报
	void onPositions(QVector<CThostFtdcInvestorPositionField>) override;
	// 委托回报
	void onOrders(QVector<CThostFtdcOrderField>) override;
	// 行情回报
	void onTick(QuoteField) override;
private:
	QMap<QString, QQueue<QuoteField> >tickMap; // 历史行情
	TradingAccount tradingAccount; // 交易账户
	QVector<CThostFtdcInvestorPositionField> positions; // 持仓情况
	QMap<QString,CThostFtdcInvestorPositionField> positionsMap; // InstrumentID映射持仓
	QVector<CThostFtdcOrderField> orders; // 报单情况
	QMap<QString, CThostFtdcOrderField>ordersMap; // OrderSysID映射报单
};
