#include "backtesting.h"

Backtesting::Backtesting(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	showDatabase();

	qRegisterMetaType<BacktestingResult>("BacktestingResult");
	connect(this, &Backtesting::sendBacktestingResult, ui.tableBacktestingResult, &TableBacktestingResult::receiveBacktestingResult);
	result = { QDate(),QDate(),0};
	emit sendBacktestingResult(result);
}

Backtesting::~Backtesting()
{

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
}
void Backtesting::sendError(QString msg)
{
	QMessageBox::critical(nullptr, "错误", msg);
}
bool Backtesting::initDatabase()
{
	if (ui.bData->count() == 0) {
		sendError("请选择数据");
		return false;
	}
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(DATABASE_PATH + ui.bData->currentText());
	if (!db.open()) {
		sendError("数据库无法打开");
		return false;
	}
	return true;
}
void Backtesting::loadData()
{

}

void Backtesting::on_start_clicked()
{
	ui.start->setEnabled(false);
	ui.stop->setEnabled(true);
	if (!initDatabase()) return;
	account = TradingAccount{ui.bFund->value(),0,0};
	result.startFund = ui.bFund->value();
	emit sendBacktestingStatus(true);
	emit sendTradingAccount(account);
}
void Backtesting::on_stop_clicked()
{
	ui.start->setEnabled(true);
	ui.stop->setEnabled(false);
	emit sendBacktestingStatus(false);
}

void Backtesting::receiveReqOrderInsert(CThostFtdcInputOrderField)
{

}
void Backtesting::receiveReqOrderAction(CThostFtdcInputOrderActionField)
{

}