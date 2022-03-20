#include "BacktestingEngine.h"

BacktestingEngine::BacktestingEngine(QObject *parent)
	: QObject(parent)
{
	db = QSqlDatabase::addDatabase("QSQLITE");
}

BacktestingEngine::~BacktestingEngine()
{

}
void BacktestingEngine::startBacktestingEngine(BacktestingForm t)
{
	form = t;
	// 初始化数据库
	db.setDatabaseName(DATABASE_PATH + form.data);
	if (!db.open()) {
		emit sendError("数据库无法打开");
		return;
	}
	// 加载数据
	QSqlQuery query;
	query.prepare("SELECT * FROM dbbardata WHERE datetime>='" + form.startTime.toString("yyyy-MM-dd") + "' AND datetime<='" + form.endTime.toString("yyyy-MM-dd")+"'");
	query.exec();
	while (query.next()) {
		kLines.push_back(KLine{
			query.value(1).toString(),
			query.value(2).toString(),
			query.value(3).toDate(),
			query.value(5).toDouble(),
			query.value(6).toDouble(),
			query.value(7).toDouble(),
			query.value(8).toDouble(),
			query.value(9).toDouble(),
			query.value(10).toDouble(),
			query.value(11).toDouble()
		});
	}
	// 设置初始资金
	account = TradingAccount{ form.fund,0,0 };
	result.startFund = form.fund;
	emit sendTradingAccount(account);
	db.close();
}

void BacktestingEngine::receiveReqOrderInsert(CThostFtdcInputOrderField)
{
}

void BacktestingEngine::receiveReqOrderAction(CThostFtdcInputOrderActionField)
{
}
