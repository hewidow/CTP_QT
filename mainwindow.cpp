#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    userStatus=new QLabel("当前用户：未登录",this);
    mdStatus=new LedLabel(this,"行情连接");
    tdStatus=new LedLabel(this,"交易连接");
    ui->statusbar->addPermanentWidget(userStatus);
    ui->statusbar->addPermanentWidget(mdStatus);
    ui->statusbar->addPermanentWidget(tdStatus);


    init();

    // showMaximized();
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

    // 信号槽连接
    /* mdApi */
    connect(&engine.mdApi,&MdApi::sendError,this,&MainWindow::receiveError);
    connect(&engine.mdApi,&MdApi::sendConnectionStatus,this,&MainWindow::receiveMdConnectionStatus);
    connect(&engine.mdApi,&MdApi::sendRspLogin,this,&MainWindow::receiveRspLoginMd);
    connect(&engine.mdApi,&MdApi::sendRtnDepthMarketData,ui->quoteTable,&QuoteTable::receiveRtnDepthMarketData);

    /* tdApi */
    connect(&engine.tdApi,&TdApi::sendError,this,&MainWindow::receiveError);
    connect(&engine.tdApi,&TdApi::sendConnectionStatus,this,&MainWindow::receiveTdConnectionStatus);
    connect(&engine.tdApi,&TdApi::sendRspLogin,this,&MainWindow::receiveRspLoginTd);
    connect(&engine.tdApi,&TdApi::sendAllInstruments,&engine,&Engine::receiveAllInstruments);
    connect(&engine.tdApi,&TdApi::sendTradingAccount,ui->fundTable,&FundTable::receiveTradingAccount);
    connect(&engine.tdApi,&TdApi::sendInvestorPositions,ui->posTable,&PosTable::receiveInvestorPositions);
    connect(&engine.tdApi,&TdApi::sendOrders,ui->entrustTable,&EntrustTable::receiveOrders);

    /* other */
    connect(this,&MainWindow::sendLog,this,&MainWindow::receiveLog);
    connect(&login,&Login::sendLoginField,&engine,&Engine::receiveLoginField);
    connect(&trade,&Trade::sendReqOrderInsert,&engine,&Engine::receiveReqOrderInsert);

    login.show();
    engine.start(); // 开启子线程
}
void MainWindow::receiveError(QString msg)
{
    QMessageBox::critical(this,"错误",msg);
}
void MainWindow::receiveMdConnectionStatus(bool status)
{
    mdStatus->setColor(status?"green":"red");
}

void MainWindow::receiveTdConnectionStatus(bool status)
{
    tdStatus->setColor(status?"green":"red");
}
void MainWindow::receiveRspLoginMd(CThostFtdcRspUserLoginField u)
{
    isMdLogin=true;
    if (isMdLogin&&isTdLogin) loginDone();
}
void MainWindow::receiveRspLoginTd(CThostFtdcRspUserLoginField u)
{
    isTdLogin=true;
    userInfo=u;
    if (isMdLogin&&isTdLogin) loginDone();
}
void MainWindow::loginDone() {
    login.close();
    showMaximized();
    userStatus->setText("当前用户："+QString(userInfo.UserID));
    iDebug<<"当前用户"<<userInfo.UserID<<"前置编号"<<userInfo.FrontID<<"会话编号"<<userInfo.SessionID;
    engine.tradeInit();
    engine.getAccountDetail();
    engine.getQuotes();
}
void MainWindow::receiveLog(QString msg)
{
    ui->logText->append(msg);
}
void MainWindow::on_action_triggered()
{
    engine.getAccountDetail();
}

void MainWindow::on_action_2_triggered()
{
    engine.tdApi.test2();
}

void MainWindow::on_action_5_triggered()
{
    trade.showDialog(userInfo);
}
