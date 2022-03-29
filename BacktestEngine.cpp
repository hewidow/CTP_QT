#include "BacktestEngine.h"

BacktestEngine::BacktestEngine(QObject* parent)
	: QObject(parent)
{
	// 以消息队列的形式推送行情数据，这样可以不阻塞策略发回来的信号
	connect(this, &BacktestEngine::sendData, this, &BacktestEngine::receiveData, Qt::QueuedConnection);
	if (QSqlDatabase::contains("QSQLITE_CONNECTION")) db = QSqlDatabase::database("QSQLITE_CONNECTION");
	else db = QSqlDatabase::addDatabase("QSQLITE", "QSQLITE_CONNECTION");
}

BacktestEngine::~BacktestEngine()
{
	if (db.isOpen()) db.close();
}
void BacktestEngine::receiveStartBacktestEngine(BacktestForm t)
{
	emit sendBacktestProgress(0);
	form = t;
	result.startTime = form.startTime;
	result.endTime = form.endTime;
	// 初始化数据库
	db.setDatabaseName(DATABASE_PATH + form.databaseName);
	if (!db.open()) {
		emit sendError("数据库无法打开");
		emit sendStopBacktestEngine(result);
		return;
	}
	// 加载合约信息
	QSqlQuery query = QSqlQuery(db);
	query.prepare("SELECT symbol,exchange FROM dbbaroverview");
	query.exec();
	while (query.next()) {
		instruments[query.value(1).toString()] = InstrumentForm{
			query.value(0).toString(),
			query.value(1).toString(),
			0
		};
	}
	query.prepare("SELECT count(1) FROM dbbardata WHERE datetime>='" + form.startTime.toString("yyyy-MM-dd") + "' AND datetime<='" + form.endTime.toString("yyyy-MM-dd") + "'");
	query.exec();
	int rowTotal = INT_MAX;
	while (query.next()) {
		rowTotal = query.value(0).toInt();
	}
	// 加载k线数据
	query.prepare("SELECT * FROM dbbardata WHERE datetime>='" + form.startTime.toString("yyyy-MM-dd") + "' AND datetime<='" + form.endTime.toString("yyyy-MM-dd") + "'");
	query.exec();
	int rowCount = 0;
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
		++rowCount;
		emit sendBacktestProgress(10ll * rowCount / rowTotal);
		if (rowCount % 5000 == 0 && QThread::currentThread()->isInterruptionRequested()) return;
	}
	db.close();
	emit sendBacktestProgress(10);
	// 设置初始资金
	account = TradingAccount{ form.startFund,0,0 };
	result.startFund = result.endFund = form.startFund;
	emit sendTradingAccount(account);
	qSort(kLines.begin(), kLines.end(), [](const KLine& a, const KLine& b) {
		return a.dateTime < b.dateTime;
		});
	// 发送行情
	emit sendData();
}
void BacktestEngine::receiveReqOrderInsert(CThostFtdcInputOrderField t)
{
	if (instruments.count(t.InstrumentID)) {
		CThostFtdcOrderField o = { 0 };
		strcpy_s(o.InstrumentID, t.InstrumentID);
		strcpy_s(o.ExchangeID, t.ExchangeID);
		strcpy_s(o.OrderSysID, QString::number(orderSysID++).toStdString().c_str());
		o.Direction = t.Direction;
		strcpy_s(o.CombOffsetFlag, t.CombOffsetFlag);
		o.LimitPrice = t.LimitPrice;
		o.VolumeTotalOriginal = t.VolumeTotalOriginal;
		o.OrderStatus = THOST_FTDC_OST_NoTradeQueueing;
		orders.push_back(o);
	}
	emit sendInvestorPositions(pos);
	emit sendOrders(orders);
	cleanOrders();
}

void BacktestEngine::receiveReqOrderAction(CThostFtdcInputOrderActionField t)
{
	for (auto& order : orders) {
		if (order.OrderSysID == t.OrderSysID) {
			if (order.OrderStatus == THOST_FTDC_OST_NoTradeQueueing) {
				switch (t.ActionFlag)
				{
				case THOST_FTDC_AF_Delete:
					order.OrderStatus = THOST_FTDC_OST_Canceled;
					break;
				default:
					break;
				}
			}
			break;
		}
	}
	emit sendInvestorPositions(pos);
	emit sendOrders(orders);
	cleanOrders();
}
void BacktestEngine::cleanOrders() {
	QVector<CThostFtdcOrderField>newOrders;
	for (auto order : orders) {
		if (order.OrderStatus == THOST_FTDC_OST_NoTradeQueueing) {
			newOrders.push_back(order);
		}
	}
	orders = newOrders;
}
void BacktestEngine::solveOrders() {
	bool change = false;
	for (auto& order : orders) {
		if (order.OrderStatus == THOST_FTDC_OST_Canceled) continue;
		double nowPrice = instruments[order.InstrumentID].Price;
		if (order.CombOffsetFlag[0] == THOST_FTDC_OF_Open) {
			if (order.LimitPrice * order.VolumeTotalOriginal > result.endFund) {
				change = true;
				order.OrderStatus = THOST_FTDC_OST_Canceled;
			}
			else if (nowPrice <= order.LimitPrice) {
				change = true;
				result.endFund -= nowPrice * order.VolumeTotalOriginal;
				int j = 0;
				for (j = 0; j < pos.size(); ++j) {
					if (pos[j].InstrumentID == order.InstrumentID) {
						pos[j].Position += order.VolumeTotalOriginal;
						break;
					}
				}
				if (j == pos.size()) {
					CThostFtdcInvestorPositionField t = { 0 };
					strcpy_s(t.InstrumentID, order.InstrumentID);
					t.Position = order.VolumeTotalOriginal;
					pos.push_back(t);
				}
				order.OrderStatus = THOST_FTDC_OST_AllTraded;
			}
		}
		else {
			for (auto& p : pos) {
				if (p.InstrumentID == order.InstrumentID) {
					if (p.Position < order.VolumeTotalOriginal) {
						change = true;
						order.OrderStatus = THOST_FTDC_OST_Canceled;
					}
					else if (order.LimitPrice <= nowPrice) {
						change = true;
						p.Position -= order.VolumeTotalOriginal;
						result.endFund += order.VolumeTotalOriginal * nowPrice;
						order.OrderStatus = THOST_FTDC_OST_AllTraded;
					}
					break;
				}
			}
		}
	}
	if (change) {
		emit sendInvestorPositions(pos);
		emit sendOrders(orders);
	}
}
void BacktestEngine::receiveData()
{
	solveOrders();
	if (kLinesP < kLines.size()) {
		emit sendKLine(kLines[kLinesP]);
		instruments[kLines[kLinesP].InstrumentID].Price = kLines[kLinesP].closePrice;
		++kLinesP;
	}
	if (kLines.size() > 0) emit sendBacktestProgress(10 + 90ll * kLinesP / kLines.size());
	if (kLinesP < kLines.size()) {
		emit sendData();
	}
	else {
		for (auto& p : pos) {
			result.endFund += instruments[p.InstrumentID].Price * p.Position;
		}
		emit sendBacktestProgress(100);
		emit sendStopBacktestEngine(result);
	}
	// iDebug << kLinesP << " " << kLines.size() << " " << orders.size();
}
