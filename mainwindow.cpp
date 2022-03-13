﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    userStatus=new QLabel("当前用户：未登录 ",this);
    mdStatus=new LedLabel(this, "red", "行情未连接");
    tdStatus=new LedLabel(this, "red", "交易未连接");
    ui->statusbar->addPermanentWidget(userStatus);
    ui->statusbar->addPermanentWidget(mdStatus);
    ui->statusbar->addPermanentWidget(tdStatus);

    ui->logText->document()->setMaximumBlockCount(LOG_MAX_ROW_COUNT); // 设置log最大行数

    init();

    showMaximized();
}
MainWindow::~MainWindow()
{
    engine.release();
    delete ui;
}

void MainWindow::logOutput(const QString &msg)
{
    emit sendLog(msg);
}
void MainWindow::init()
{
    // 注册自定义类型
    qRegisterMetaType<CThostFtdcRspUserLoginField>("CThostFtdcRspUserLoginField");
    qRegisterMetaType<QuoteField>("QuoteField");
    qRegisterMetaType<QVector<InstrumentField>>("QVector<InstrumentField>");
    qRegisterMetaType<TradingAccount>("TradingAccount");
    qRegisterMetaType<QVector<CThostFtdcInvestorPositionField>>("QVector<CThostFtdcInvestorPositionField>");
    qRegisterMetaType<QVector<CThostFtdcOrderField>>("QVector<CThostFtdcOrderField>");
    qRegisterMetaType<CThostFtdcOrderField>("CThostFtdcOrderField");
    qRegisterMetaType<CThostFtdcInputOrderField>("CThostFtdcInputOrderField");
    qRegisterMetaType<CThostFtdcInputOrderActionField>("CThostFtdcInputOrderActionField");

    // 信号槽连接

    /* MdApi */
    connect(&engine.mdApi,&MdApi::sendError,this,&MainWindow::receiveError);
    connect(&engine.mdApi,&MdApi::sendConnectionStatus,this,&MainWindow::receiveMdConnectionStatus);
    connect(&engine.mdApi,&MdApi::sendRspLogin,this,&MainWindow::receiveRspLoginMd);
    connect(&engine.mdApi,&MdApi::sendRtnDepthMarketData,ui->quoteTable,&QuoteTable::receiveRtnDepthMarketData);
    connect(&engine.mdApi, &MdApi::sendRtnDepthMarketData, &strategy.strategyModel, &StrategyModel::receiveRtnDepthMarketData);

    /* TdApi & ui */
    connect(&engine.tdApi,&TdApi::sendError,this,&MainWindow::receiveError);
    connect(&engine.tdApi,&TdApi::sendConnectionStatus,this,&MainWindow::receiveTdConnectionStatus);
	connect(&engine.tdApi,&TdApi::sendReqAuthenticateCommand, &engine, &Engine::receiveReqAuthenticateCommand);
	connect(&engine.tdApi,&TdApi::sendLoginCommand, &engine, &Engine::receiveLoginCommand);
    connect(&engine.tdApi,&TdApi::sendRspLogin,this,&MainWindow::receiveRspLoginTd);
    connect(&engine.tdApi,&TdApi::sendAllInstruments,&engine,&Engine::receiveAllInstruments);
    connect(&engine.tdApi,&TdApi::sendTradingAccount,ui->fundTable,&FundTable::receiveTradingAccount);
    connect(&engine.tdApi,&TdApi::sendInvestorPositions,ui->posTable,&PosTable::receiveInvestorPositions);
    connect(&engine.tdApi,&TdApi::sendOrders,ui->entrustTable,&EntrustTable::receiveOrders);
    connect(&engine.tdApi,&TdApi::sendOrderChange,&engine,&Engine::receiveOrderChange);

    /* TdApi & StrategyModel */
    connect(&engine.tdApi, &TdApi::sendAllInstruments, &strategy.strategyModel, &StrategyModel::receiveAllInstruments);
    connect(&engine.tdApi, &TdApi::sendRspLogin, &strategy.strategyModel, &StrategyModel::receiveRspLoginTd);
    connect(&engine.tdApi, &TdApi::sendTradingAccount, &strategy.strategyModel, &StrategyModel::receiveTradingAccount);
    connect(&engine.tdApi, &TdApi::sendInvestorPositions, &strategy.strategyModel, &StrategyModel::receiveInvestorPositions);
    connect(&engine.tdApi, &TdApi::sendOrders, &strategy.strategyModel, &StrategyModel::receiveOrders);

    /* Engine */
    connect(&engine, &Engine::sendError, this, &MainWindow::receiveError);

    /* Login */
    connect(&login, &Login::sendLoginField, &engine, &Engine::receiveLoginField);

    /* Trade */
    connect(&trade,&Trade::sendReqOrderInsert,&engine,&Engine::receiveReqOrderInsert);
    connect(&trade,&Trade::sendReqOrderAction,&engine,&Engine::receiveReqOrderAction);

    /* StrategyModel */
    connect(&strategy.strategyModel, &StrategyModel::sendReqOrderInsert, &engine, &Engine::receiveReqOrderInsert);
    connect(&strategy.strategyModel, &StrategyModel::sendReqOrderAction, &engine, &Engine::receiveReqOrderAction);

    /* MainWindow */
    connect(this, &MainWindow::sendLog, this, &MainWindow::receiveLog);

    login.show();
}
void MainWindow::receiveError(QString msg)
{
    QMessageBox::critical(nullptr,"错误",msg);
}
void MainWindow::receiveMdConnectionStatus(bool status)
{
    if (status) mdStatus->setStatus("green", "行情已连接");
    else mdStatus->setStatus("red", "行情未连接");
}

void MainWindow::receiveTdConnectionStatus(bool status)
{
    if (status) tdStatus->setStatus("green", "交易已连接");
    else tdStatus->setStatus("red", "交易未连接");
}
void MainWindow::receiveRspLoginMd(CThostFtdcRspUserLoginField u)
{
    isMdLogin=true;
    if (isMdLogin&&isTdLogin) loginDone();
}
void MainWindow::receiveRspLoginTd(CThostFtdcRspUserLoginField u)
{
    isTdLogin=true;
    engine.userInfo=u;
    if (isMdLogin&&isTdLogin) loginDone();
}
void MainWindow::loginDone() {
    login.close();
    showMaximized();
    userStatus->setText("当前用户："+QString(engine.userInfo.UserID)+" ");
    iDebug<<"当前用户"<< engine.userInfo.UserID<<"前置编号"<< engine.userInfo.FrontID<<"会话编号"<< engine.userInfo.SessionID;
    engine.tradeInit();
    engine.getAccountDetail();
    engine.getQuotes();
}
void MainWindow::receiveLog(QString msg)
{
    ui->logText->append(msg);
}

void MainWindow::on_trade_triggered()
{
    trade.show();
}
void MainWindow::on_strategy_triggered()
{
    strategy.show();
}
void MainWindow::on_test1_triggered()
{
    QDir dir;
    if (!dir.exists(STRATEGY_PATH)) dir.mkpath(STRATEGY_PATH);

    dir.setPath(STRATEGY_PATH);

    dir.setFilter(QDir::Files);
    QStringList filters;
    filters << "*.cpp";
    dir.setNameFilters(filters);
    QStringList s=dir.entryList();
    iDebug << s;
}

void MainWindow::on_test2_triggered()
{
    engine.getAccountDetail();
}