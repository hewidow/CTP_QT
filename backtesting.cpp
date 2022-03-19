#include "backtesting.h"

Backtesting::Backtesting(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

Backtesting::~Backtesting()
{
}
void Backtesting::receiveReqOrderInsert(CThostFtdcInputOrderField)
{

}
void Backtesting::receiveReqOrderAction(CThostFtdcInputOrderActionField)
{
}

void Backtesting::loadData()
{
}

void Backtesting::closeEvent(QCloseEvent* event)
{
	on_stop_clicked();
}
void Backtesting::on_start_clicked()
{
	loadData();
	emit sendBacktestingStatus(true);
}
void Backtesting::on_stop_clicked()
{
	emit sendBacktestingStatus(false);
}