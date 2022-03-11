#include "strategymodel.h"

StrategyModel::StrategyModel()
{

}

StrategyModel::~StrategyModel()
{
	pause();
}

void StrategyModel::run()
{
	QTimer timer;
	bool res = connect(&timer, &QTimer::timeout, this, &StrategyModel::timeOut);
	timer.start(STRATEGY_MODEL_RUN_INTERVAL);
	exec();
}
#include "strategymodel.h"

void StrategyModel::timeOut() {
	iDebug << "0s";
}

void StrategyModel::pause()
{
	quit();
	wait();
}
