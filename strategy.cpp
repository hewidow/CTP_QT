#include "strategy.h"

Strategy::Strategy(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.pause->setVisible(false);

	//QDir dir;
	//if (!dir.exists(STRATEGY_PATH)) dir.mkpath(STRATEGY_PATH);
	//dir.setPath(STRATEGY_PATH);
	//dir.setFilter(QDir::Files);
	//QStringList filters;
	//filters << "*.cpp";
	//dir.setNameFilters(filters);
	//QStringList names = dir.entryList();
	//model= new QStandardItemModel(this);
	//for (auto &name : names) {
	//    model->appendRow(new QStandardItem(name));
	//}
	strategies.push_back(new StrategyExample());
	strategies.push_back(new StrategyMultiBreed());
	model = new QStandardItemModel(this);
	for (auto& it : strategies) {
		model->appendRow(new QStandardItem(it->name()));
	}
	ui.listView->setModel(model); // 显示STRATEGY_PATH路径下所有后缀为cpp的文件名
	ui.listView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
	connect(ui.listView, &QListView::doubleClicked, this, &Strategy::on_backtest_clicked); // 绑定双击事件

	// 连接回测的信号槽，根据sender()来筛选信号
	connect(&backtest, &Backtest::sendBacktestStatus, this, &Strategy::receiveBacktestStatus);
	connect(&backtest, &Backtest::sendTradingAccount, this, &Strategy::receiveTradingAccount);
	connect(&backtest, &Backtest::sendInvestorPositions, this, &Strategy::receiveInvestorPositions);
	connect(&backtest, &Backtest::sendOrders, this, &Strategy::receiveOrders);
	connect(&backtest, &Backtest::sendRtnDepthMarketData, this, &Strategy::receiveRtnDepthMarketData);
	connect(&backtest, &Backtest::sendKLine, this, &Strategy::receiveKLine);
	connect(this, &Strategy::sendReqOrderInsertBacktest, &backtest, &Backtest::sendReqOrderInsert);
	connect(this, &Strategy::sendReqOrderActionBacktest, &backtest, &Backtest::sendReqOrderAction);

}

Strategy::~Strategy()
{

}

void Strategy::on_add_clicked()
{
	iDebug << "add";
}
void Strategy::on_del_clicked()
{
	iDebug << "del";
}

void Strategy::on_edit_clicked()
{
	iDebug << "edit";
	//QString path = STRATEGY_PATH + ui.listView->currentIndex().data().toString();
	//QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(path).absoluteFilePath()));
}
bool Strategy::checkCurrentSelectIndex()
{
	if (ui.listView->currentIndex().row() == -1) {
		QMessageBox::critical(nullptr, "错误", "请先选择一项策略");
		return false;
	}
	return true;
}
void Strategy::on_start_clicked() {
	if (!checkCurrentSelectIndex() || strategyRunning) return;
	cur = ui.listView->currentIndex().row();
	emit sendStrategyStatus(strategies[cur]->name());
	connect(strategies[cur], &StrategyBase::sendReqOrderInsert, this, &Strategy::receiveReqOrderInsert);
	connect(strategies[cur], &StrategyBase::sendReqOrderAction, this, &Strategy::receiveReqOrderAction);
	strategies[cur]->backtest = true;
	strategies[cur]->onStart();
	ui.start->setVisible(false);
	ui.pause->setVisible(true);
	strategyRunning = true;
	strategyBacktest = true;
}

void Strategy::on_pause_clicked()
{
	if (!strategyRunning) return;
	strategyRunning = false;
	strategyBacktest = false;
	strategies[cur]->onStop();
	strategies[cur]->disconnect(); // 断开所有信号槽
	strategies[cur]->backtest = false;
	cur = -1;
	ui.start->setVisible(true);
	ui.pause->setVisible(false);
	emit sendStrategyStatus("无");
}
void Strategy::on_backtest_clicked()
{
	if (!checkCurrentSelectIndex()) return;
	on_pause_clicked();
	backtest.exec();
}
void Strategy::receiveBacktestStatus(bool status)
{
	if (status) on_start_clicked();
	else on_pause_clicked();
}
bool Strategy::checkEnvironment()
{
	if (!strategyRunning) return false;
	const Backtest* obj = qobject_cast<Backtest*>(sender());
	if (strategyBacktest && obj == nullptr) return false;
	return true;
}
void Strategy::receiveRtnDepthMarketData(QuoteField t)
{
	if (!checkEnvironment()) return;
	strategies[cur]->onTick(t);
}
void Strategy::receiveTradingAccount(TradingAccount t)
{
	if (!checkEnvironment()) return;
	strategies[cur]->onAccount(t);
}
void Strategy::receiveInvestorPositions(QVector<CThostFtdcInvestorPositionField> t)
{
	if (!checkEnvironment()) return;
	strategies[cur]->onPositions(t);
}
void Strategy::receiveOrders(QVector<CThostFtdcOrderField> t)
{
	if (!checkEnvironment()) return;
	strategies[cur]->onOrders(t);
}
void Strategy::receiveKLine(KLine t)
{
	if (!checkEnvironment()) return;
	strategies[cur]->onKLine(t);
}
void Strategy::receiveReqOrderInsert(CThostFtdcInputOrderField t)
{
	if (!strategyRunning) return;
	if (strategyBacktest) emit sendReqOrderInsertBacktest(t);
	else emit sendReqOrderInsert(t);
}
void Strategy::receiveReqOrderAction(CThostFtdcInputOrderActionField t)
{
	if (!strategyRunning) return;
	if (strategyBacktest)emit sendReqOrderActionBacktest(t);
	else emit sendReqOrderAction(t);
}


