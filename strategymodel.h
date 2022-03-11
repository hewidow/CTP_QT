#pragma once
#include <QThread>
#include <QTimer>
#include "global.h"
#include "debug.h"

#ifdef STRATEGY_MODEL_EXPORTS
#define STRATEGY_MODEL_API __declspec(dllexport)
#else
#define STRATEGY_MODEL_API __declspec(dllimport)
#endif

class StrategyModel:public QThread
{
	Q_OBJECT
public:
	StrategyModel();
	~StrategyModel();
	void run() override;
	// ÔÝÍ£×ÓÏß³Ì
	void pause();
public slots:
	STRATEGY_MODEL_API void timeOut();
signals:
	void sendReqOrderInsert(CThostFtdcInputOrderField);
	void sendReqOrderAction(CThostFtdcInputOrderActionField);
private:
	bool working = true;
};
