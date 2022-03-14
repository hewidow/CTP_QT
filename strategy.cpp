#include "strategy.h"

Strategy::Strategy(QWidget *parent)
	: QWidget(parent)
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
void Strategy::on_start_clicked() {
    cur = ui.listView->currentIndex().row();
    if (cur == -1) {
        QMessageBox::critical(nullptr, "错误", "请先选择一项策略");
        return;
    }
    connect(strategies[cur], &StrategyBase::sendReqOrderInsert, this, &Strategy::receiveReqOrderInsert);
    connect(strategies[cur], &StrategyBase::sendReqOrderAction, this, &Strategy::receiveReqOrderAction);
    strategies[cur]->onStart();
    ui.start->setVisible(false);
    ui.pause->setVisible(true);
}

void Strategy::on_pause_clicked()
{
    strategies[cur]->onStop();
    strategies[cur]->disconnect(); // 断开所有连接
    cur = -1;
    ui.start->setVisible(true);
    ui.pause->setVisible(false);
}
void Strategy::receiveRtnDepthMarketData(QuoteField q)
{
    if (cur == -1) return;
    strategies[cur]->onTick(q);
}
void Strategy::receiveTradingAccount(TradingAccount t)
{
    if (cur == -1) return;
    strategies[cur]->onAccount(t);
}
void Strategy::receiveInvestorPositions(QVector<CThostFtdcInvestorPositionField> p)
{
    if (cur == -1) return;
    strategies[cur]->onPositions(p);
}
void Strategy::receiveOrders(QVector<CThostFtdcOrderField> o)
{
    if (cur == -1) return;
    strategies[cur]->onOrders(o);
}
void Strategy::receiveAllInstruments(QVector<InstrumentField>i)
{
    if (cur == -1) return;
}
void Strategy::receiveReqOrderInsert(CThostFtdcInputOrderField o)
{
    if (cur == -1) return;
    emit sendReqOrderInsert(o);
}
void Strategy::receiveReqOrderAction(CThostFtdcInputOrderActionField a)
{
    if (cur == -1) return;
    emit sendReqOrderAction(a);
}