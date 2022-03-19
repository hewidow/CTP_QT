#include "strategy.h"

Strategy::Strategy(QWidget *parent)
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
    strategies.push_back(new StrategyExample());
    model = new QStandardItemModel(this);
    for (auto& it : strategies) {
        model->appendRow(new QStandardItem(it->name()));
    }
    ui.listView->setModel(model); // 显示STRATEGY_PATH路径下所有后缀为cpp的文件名
    ui.listView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    connect(ui.listView, &QListView::doubleClicked, this, &Strategy::on_edit_clicked); // 绑定双击事件

    // 连接回测的信号槽，根据sender()来筛选信号
    connect(&backtesting, &Backtesting::sendBacktestingStatus, this, &Strategy::receiveBacktestingStatus);
    connect(&backtesting, &Backtesting::sendRtnDepthMarketData, this, &Strategy::receiveRtnDepthMarketData);
    connect(&backtesting, &Backtesting::sendTradingAccount, this, &Strategy::receiveTradingAccount);
    connect(&backtesting, &Backtesting::sendInvestorPositions, this, &Strategy::receiveInvestorPositions);
    connect(&backtesting, &Backtesting::sendOrders, this, &Strategy::receiveOrders);
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
    QString path = STRATEGY_PATH + ui.listView->currentIndex().data().toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(path).absoluteFilePath()));
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
    strategies[cur]->onStart();
    ui.start->setVisible(false);
    ui.pause->setVisible(true);
    strategyRunning = true;
}

void Strategy::on_pause_clicked()
{
    if (!strategyRunning) return;
    strategyRunning = false;
    strategyBacktesting = false;
    strategies[cur]->onStop();
    strategies[cur]->disconnect(); // 断开所有信号槽
    cur = -1;
    ui.start->setVisible(true);
    ui.pause->setVisible(false);
    emit sendStrategyStatus("无");
}
void Strategy::on_backtesting_clicked()
{
    if (!checkCurrentSelectIndex()) return;
    on_pause_clicked();
    strategyBacktesting = true;
    backtesting.exec();
}
bool Strategy::checkEnvironment()
{
    if (!strategyRunning) return false;
    const Backtesting* obj = qobject_cast<Backtesting*>(sender());
    if (strategyBacktesting && obj==nullptr) return false;
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
void Strategy::receiveReqOrderInsert(CThostFtdcInputOrderField t)
{
    if (!strategyRunning) return;
    if (strategyBacktesting) backtesting.receiveReqOrderInsert(t);
    else emit sendReqOrderInsert(t);
}
void Strategy::receiveReqOrderAction(CThostFtdcInputOrderActionField t)
{
    if (!strategyRunning) return;
    if (strategyBacktesting) backtesting.receiveReqOrderAction(t);
    else emit sendReqOrderAction(t);
}

void Strategy::receiveBacktestingStatus(bool status)
{
    if (status) on_start_clicked();
    else on_pause_clicked();
}
