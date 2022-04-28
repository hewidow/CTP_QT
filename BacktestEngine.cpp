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
	query.prepare("SELECT symbol,exchange FROM dbbaroverview WHERE NOT(end<'" + form.startTime.toString("yyyy-MM-dd") + "' OR start>'" + form.endTime.toString("yyyy-MM-dd") + "');");
	query.exec();
	while (query.next()) {
		instruments[query.value(0).toString()] = InstrumentForm{
			query.value(0).toString(),
			query.value(1).toString(),
			0
		};
	}
	query.prepare("SELECT count(1) FROM dbbardata WHERE datetime>='" + form.startTime.toString("yyyy-MM-dd") + "' AND datetime<='" + form.endTime.toString("yyyy-MM-dd") + "';");
	query.exec();
	int rowTotal = INT_MAX;
	while (query.next()) {
		rowTotal = query.value(0).toInt();
	}
	// 加载k线数据
	query.prepare("SELECT * FROM dbbardata WHERE datetime>='" + form.startTime.toString("yyyy-MM-dd") + "' AND datetime<='" + form.endTime.toString("yyyy-MM-dd") + "';");
	query.exec();
	int rowCount = 0;
	while (query.next()) {
		kLines.push_back(KLine{
			query.value(1).toString(),
			query.value(2).toString(),
			query.value(3).toDateTime(),
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
	account = TradingAccount{ form.startFund,0,form.startFund,0 };
	result.startFund = result.endFund = form.startFund;
	result.handlingFeeRate = form.handlingFeeRate;
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
	cleanPos();
	cleanOrders();
}

void BacktestEngine::receiveReqOrderAction(CThostFtdcInputOrderActionField t)
{
	for (auto& order : orders) {
		if (strcmp(order.OrderSysID, t.OrderSysID) == 0) {
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
	cleanPos();
	cleanOrders();
}
void BacktestEngine::cleanPos() {
	QVector<CThostFtdcInvestorPositionField>newPos;
	for (auto p : pos) {
		if (p.OpenVolume - p.CloseVolume > 0) {
			newPos.push_back(p);
		}
	}
	pos.clear();
	pos = newPos;
}
void BacktestEngine::cleanOrders() {
	QVector<CThostFtdcOrderField>newOrders;
	for (auto order : orders) {
		if (order.OrderStatus == THOST_FTDC_OST_NoTradeQueueing) {
			newOrders.push_back(order);
		}
	}
	orders.clear();
	orders = newOrders;
}
int BacktestEngine::findPosIndex(const char* InstrumentID) {
	for (int i = 0; i < pos.size(); ++i) {
		if (strcmp(pos[i].InstrumentID, InstrumentID) == 0) {
			return i;
		}
	}
	return -1;
}
void BacktestEngine::solveOrders() {
	bool change = false;
	for (auto& order : orders) {
		if (order.OrderStatus == THOST_FTDC_OST_Canceled) continue;
		double nowPrice = instruments[order.InstrumentID].Price;
		double nowPriceL = nowPrice - nowPrice * 0.05;
		double nowPriceR = nowPrice + nowPrice * 0.05;
		// 开仓
		if (order.CombOffsetFlag[0] == THOST_FTDC_OF_Open) {
			// 超过当前价格±5%范围（实际为当日价格涨跌幅限制） 或 钱不够 为废单
			if (order.LimitPrice < nowPriceL || order.LimitPrice > nowPriceR || std::min(order.LimitPrice, nowPrice) * order.VolumeTotalOriginal * (1.0 + result.handlingFeeRate) > result.endFund) {
				change = true;
				order.OrderStatus = THOST_FTDC_OST_Canceled;
			}
			else if (nowPrice <= order.LimitPrice) { // 如果报单价格大于等于当前价格，即可开仓
				change = true;
				result.endFund -= nowPrice * order.VolumeTotalOriginal * (1.0 + result.handlingFeeRate);
				result.totalHandlingFee += nowPrice * order.VolumeTotalOriginal * result.handlingFeeRate;
				int posIndex = findPosIndex(order.InstrumentID);
				if (posIndex != -1) {
					pos[posIndex].OpenVolume += order.VolumeTotalOriginal;
					pos[posIndex].OpenAmount += nowPrice * order.VolumeTotalOriginal;
				}
				else {
					CThostFtdcInvestorPositionField t = { 0 };
					strcpy_s(t.InstrumentID, order.InstrumentID);
					t.OpenVolume = order.VolumeTotalOriginal;
					t.OpenAmount = nowPrice * order.VolumeTotalOriginal;
					pos.push_back(t);
				}
				order.OrderStatus = THOST_FTDC_OST_AllTraded;
			}
		}
		else if (order.CombOffsetFlag[0] == THOST_FTDC_OF_Close) { // 平仓
			int posIndex = findPosIndex(order.InstrumentID);
			if (posIndex != -1) {
				// 超过当前价格±5%范围（实际为当日价格涨跌幅限制） 或 报单平仓量超过持仓量 为废单
				if (order.LimitPrice < nowPriceL || order.LimitPrice > nowPriceR || pos[posIndex].OpenVolume - pos[posIndex].CloseVolume < order.VolumeTotalOriginal) {
					change = true;
					order.OrderStatus = THOST_FTDC_OST_Canceled;
				}
				else if (order.LimitPrice <= nowPrice) { // 如果报单价格小于等于当前价格，即可平仓
					change = true;
					pos[posIndex].CloseVolume += order.VolumeTotalOriginal;
					pos[posIndex].CloseAmount += nowPrice * order.VolumeTotalOriginal;
					result.endFund += nowPrice * order.VolumeTotalOriginal * (1 - result.handlingFeeRate);
					result.totalHandlingFee += nowPrice * order.VolumeTotalOriginal * result.handlingFeeRate;
					order.OrderStatus = THOST_FTDC_OST_AllTraded;
					++result.totalTransactions;
					if (nowPrice * pos[posIndex].OpenVolume >= pos[posIndex].OpenAmount) ++result.profitTransactions;
					else ++result.lossTransactions;
				}
			}
		}
	}
	// 持仓变化或者报单状态变化了才发送
	if (change) {
		emit sendInvestorPositions(pos);
		emit sendOrders(orders);
		// 传完了再删，保持一个消息单位的延迟
		cleanPos();
		cleanOrders();
	}
}
void BacktestEngine::receiveData()
{
	solveOrders();
	if (kLinesP < kLines.size()) {
		instruments[kLines[kLinesP].InstrumentID].Price = kLines[kLinesP].closePrice;
		emit sendKLine(kLines[kLinesP]);
		// 当是这个时刻最后一条k线数据时
		if (kLinesP + 1 >= kLines.size() || kLines[kLinesP].dateTime != kLines[kLinesP + 1].dateTime) {
			double nowFund = result.endFund;
			for (auto& p : pos) {
				nowFund += instruments[p.InstrumentID].Price * (p.OpenVolume - p.CloseVolume) * (1 - result.handlingFeeRate);
			}
			result.maximumDrawdown = std::min(result.maximumDrawdown, nowFund - result.startFund);
			account.PositionProfit = nowFund - result.startFund;
			account.Available = result.endFund;
			account.totalAssets = account.FrozenMargin + account.Available + account.PositionProfit;
			// 每十二小时记录一次浮动盈亏数据
			if (kLines[kLinesP].dateTime.toMSecsSinceEpoch() - startTimeStamp >= (long long)1000 * 60 * 60 * 12) {
				startTimeStamp = kLines[kLinesP].dateTime.toMSecsSinceEpoch();
				chartData.floatingProfitLossData.push_back({ startTimeStamp ,Util::formatDoubleTwoDecimal(nowFund) });
				chartData.floatingProfitLossRateData.push_back({ startTimeStamp ,Util::formatDoubleTwoDecimal((nowFund - result.startFund) / result.startFund * 100) });
				double nowFuturesPrice = calcFuturesPrice();
				if (startRecord) {
					if (firstRecord) startFuturesPrice = nowFuturesPrice, firstRecord = false, iDebug << instruments.size();
					chartData.futuresPriceRateData.push_back({ startTimeStamp ,Util::formatDoubleTwoDecimal((nowFuturesPrice - startFuturesPrice) / startFuturesPrice * 100) });
				}
			}
			emit sendTradingAccount(account);
		}
		++kLinesP;
		emit sendBacktestProgress(10 + 90ll * kLinesP / kLines.size());
		emit sendData();
	}
	else {
		// 数据全过完了，强平持仓
		orders.clear();
		for (auto& p : pos) {
			CThostFtdcOrderField o = { 0 };
			strcpy_s(o.InstrumentID, p.InstrumentID);
			o.Direction = THOST_FTDC_D_Buy;
			o.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
			o.LimitPrice = instruments[p.InstrumentID].Price;
			o.VolumeTotalOriginal = p.OpenVolume - p.CloseVolume;
			o.OrderStatus = THOST_FTDC_OST_NoTradeQueueing;
			orders.push_back(o);
		}
		solveOrders();
		calcResult();
		emit sendBacktestProgress(100);
		emit sendBacktestChartData(chartData);
		emit sendStopBacktestEngine(result);
	}
}

void BacktestEngine::calcResult() {
	int totalDays = result.startTime.daysTo(result.endTime);
	result.profitRate = (result.endFund - result.startFund) / result.startFund;
	result.annualizedProfitRate = (result.endFund - result.startFund) / result.startFund * totalDays / 365;
	result.maximumDrawdownRate = result.maximumDrawdown / result.startFund;
	result.totalProfitAndLoss = result.endFund - result.startFund;
}

double BacktestEngine::calcFuturesPrice()
{
	int count = 0;
	double sum = 0;
	for (auto& it : instruments) {
		if (it.Price <= 0) return 0;
		sum += it.Price;
		++count;
	}
	startRecord = true;
	return sum / count;
}
