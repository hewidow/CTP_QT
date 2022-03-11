#pragma once
#include <QObject>

class StrategyModel:public QObject
{
	Q_OBJECT

public:
	StrategyModel();
	~StrategyModel();
signals:
	void sendReqOrderInsert();
	void sendReqOrderAction();
};
