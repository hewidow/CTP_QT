#pragma once

#include "StrategyBase.h"
#include <QQueue>
#include <cmath>

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
	QMap<QString, QQueue<KLine> >kLineMap; // ��ʷ����
	QVector<CThostFtdcInvestorPositionField> positions; // �ֲ����
	QMap<QString, CThostFtdcInvestorPositionField> positionsMap; // InstrumentIDӳ��ֲ�
	QVector<CThostFtdcOrderField> orders; // �������
	QMap<QString, CThostFtdcOrderField>ordersMap; // OrderSysIDӳ�䱨��

	int period = 61; // ��ʷ���ݱ�������
	int longPeriod = 60; // ����������
	int shortPeriod = 20; // �̾�������

	QVector<QString>instruments{ "ag2203","IC2203","jd2203" }; // ��Ҫ���׵ĺ�ԼƷ������
	QMap<QString, double>weights; // ��ʹ���ʽ��Ȩ��
	QMap<QString, double>HV; // ��ʷ������
};
