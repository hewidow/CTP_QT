#include "backtest.h"

Backtest::Backtest(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	thread = nullptr;
	engine = nullptr;

	showDatabase();

	qRegisterMetaType<BacktestResult>("BacktestResult");
	connect(this, &Backtest::sendBacktestResult, ui.tableBacktestResult, &TableBacktestResult::receiveBacktestResult);
	emit sendBacktestResult({ QDate(),QDate(),0 });
	receiveBacktestProgress(0);

	ui.webEngineView->setContextMenuPolicy(Qt::NoContextMenu);
	ui.webEngineView->setUrl(QUrl("qrc:/echarts/lineChart.html"));
	ui.webEngineView->show();
}

Backtest::~Backtest()
{
	on_stop_clicked();
}
void Backtest::closeEvent(QCloseEvent* event)
{
	on_stop_clicked();
}
void Backtest::showDatabase()
{
	QDir dir;
	if (!dir.exists(DATABASE_PATH)) dir.mkpath(DATABASE_PATH);
	dir.setPath(DATABASE_PATH);
	dir.setFilter(QDir::Files);
	QStringList filters;
	filters << "*.db";
	dir.setNameFilters(filters);
	QStringList names = dir.entryList();
	QStandardItemModel* model = new QStandardItemModel(this);
	for (auto& name : names) {
		model->appendRow(new QStandardItem(name));
	}
	ui.bDatabaseName->setModel(model);
	if (names.size() == 0) ui.start->setEnabled(false);
}
void Backtest::receiveError(QString msg)
{
	QMessageBox::critical(nullptr, "错误", msg);
}

void Backtest::receiveBacktestProgress(int val)
{
	ui.progressBar->setValue(val);
}

void Backtest::receiveStopBacktestEngine(BacktestResult t)
{
	emit sendBacktestResult(t);
	on_stop_clicked();
}

void Backtest::on_start_clicked()
{
	ui.stop->setEnabled(true);
	ui.start->setEnabled(false);

	emit sendBacktestStatus(true);

	engine = new BacktestEngine();
	thread = new QThread();
	connect(engine, &BacktestEngine::sendTradingAccount, this, &Backtest::sendTradingAccount);
	connect(engine, &BacktestEngine::sendInvestorPositions, this, &Backtest::sendInvestorPositions);
	connect(engine, &BacktestEngine::sendOrders, this, &Backtest::sendOrders);
	connect(engine, &BacktestEngine::sendRtnDepthMarketData, this, &Backtest::sendRtnDepthMarketData);
	connect(engine, &BacktestEngine::sendKLine, this, &Backtest::sendKLine);
	connect(this, &Backtest::sendReqOrderInsert, engine, &BacktestEngine::receiveReqOrderInsert);
	connect(this, &Backtest::sendReqOrderAction, engine, &BacktestEngine::receiveReqOrderAction);
	qRegisterMetaType<BacktestForm>("BacktestForm");
	connect(engine, &BacktestEngine::sendError, this, &Backtest::receiveError);
	connect(engine, &BacktestEngine::sendBacktestProgress, this, &Backtest::receiveBacktestProgress);
	connect(this, &Backtest::sendStartBacktestEngine, engine, &BacktestEngine::receiveStartBacktestEngine);
	connect(engine, &BacktestEngine::sendStopBacktestEngine, this, &Backtest::receiveStopBacktestEngine);

	engine->moveToThread(thread);
	thread->start();

	emit sendBacktestResult({ QDate(),QDate(),0 });
	emit sendStartBacktestEngine(BacktestForm{
		ui.bDatabaseName->currentText(),
		ui.bStartTime->date(),
		ui.bEndTime->date(),
		ui.bStartFund->value(),
		ui.bHandlingFeeRate->value() / 100.0
		});
}
void Backtest::on_stop_clicked()
{
	if (thread != nullptr) {
		thread->requestInterruption();
		thread->quit();
		thread->wait();
		delete thread;
		thread = nullptr;
	}
	if (engine != nullptr) {
		engine->disconnect();
		delete engine;
		engine = nullptr;
	}
	emit sendBacktestStatus(false);
	ui.start->setEnabled(true);
	ui.stop->setEnabled(false);
}
