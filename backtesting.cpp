#include "backtesting.h"

Backtesting::Backtesting(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	showDatabase();

	qRegisterMetaType<BacktestingResult>("BacktestingResult");
	connect(this, &Backtesting::sendBacktestingResult, ui.tableBacktestingResult, &TableBacktestingResult::receiveBacktestingResult);
	emit sendBacktestingResult({ QDate(),QDate(),0 });

	engine.moveToThread(&thread);
	qRegisterMetaType<BacktestingForm>("BacktestingForm");
	connect(&engine, &BacktestingEngine::sendError, this, &Backtesting::receiveError);
	connect(this, &Backtesting::startBacktestingEngine, &engine, &BacktestingEngine::startBacktestingEngine);
	connect(&engine, &BacktestingEngine::endBacktestingEngine, this, &Backtesting::endBacktestingEngine);

}

Backtesting::~Backtesting()
{
	thread.quit();
	thread.wait();
}
void Backtesting::closeEvent(QCloseEvent* event)
{
	on_stop_clicked();
}
void Backtesting::showDatabase()
{
	QDir dir;
	if (!dir.exists(DATABASE_PATH)) dir.mkpath(DATABASE_PATH);
	dir.setPath(DATABASE_PATH);
	dir.setFilter(QDir::Files);
	QStringList filters;
	filters << "*.db";
	dir.setNameFilters(filters);
	QStringList names = dir.entryList();
	QStandardItemModel *model = new QStandardItemModel(this);
	for (auto &name : names) {
	    model->appendRow(new QStandardItem(name));
	}
	ui.bData->setModel(model);
	if (names.size() == 0) ui.start->setEnabled(false);
}
void Backtesting::receiveError(QString msg)
{
	QMessageBox::critical(nullptr, "错误", msg);
}

void Backtesting::endBacktestingEngine(BacktestingResult t)
{
	emit sendBacktestingResult(t);
	on_stop_clicked();
}

void Backtesting::on_start_clicked()
{
	thread.start();
	ui.start->setEnabled(false);
	ui.stop->setEnabled(true);
	emit sendBacktestingStatus(true);
	emit sendBacktestingResult({ QDate(),QDate(),0 });
	emit startBacktestingEngine(BacktestingForm{
		ui.bData->currentText(),
		ui.bStartTime->date(),
		ui.bEndTime->date(),
		ui.bFund->value()
	});
}
void Backtesting::on_stop_clicked()
{
	ui.start->setEnabled(true);
	ui.stop->setEnabled(false);
	emit sendBacktestingStatus(false);
	thread.quit();
	thread.wait();
}
