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

    showMaximized();

}
MainWindow::~MainWindow()
{
    mdApi.release();
    tdApi.release();
    delete ui;
}

void MainWindow::logOutput(const QString &msg)
{
    emit sendLog(msg);
}
void MainWindow::init()
{
    // 注册自定义类型
    qRegisterMetaType<RspLoginField>("RspLoginField");
    qRegisterMetaType<QuoteField>("QuoteField");
    qRegisterMetaType<QVector<InstrumentField>>("QVector<InstrumentField>");
    qRegisterMetaType<TradingAccount>("TradingAccount");
    qRegisterMetaType<InvestorPosition>("InvestorPosition");
    qRegisterMetaType<Order>("Order");

    // 信号槽连接
    /* mdApi */
    connect(&mdApi,&MdApi::sendError,this,&MainWindow::receiveError);
    connect(&mdApi,&MdApi::sendConnectionStatus,this,&MainWindow::receiveMdConnectionStatus);
    connect(&mdApi,&MdApi::sendRspLogin,this,&MainWindow::receiveRspLoginMd);
    connect(&mdApi,&MdApi::sendRtnDepthMarketData,this,&MainWindow::receiveRtnDepthMarketData);

    /* tdApi */
    connect(&tdApi,&TdApi::sendError,this,&MainWindow::receiveError);
    connect(&tdApi,&TdApi::sendConnectionStatus,this,&MainWindow::receiveTdConnectionStatus);
    connect(&tdApi,&TdApi::sendRspLogin,this,&MainWindow::receiveRspLoginTd);
    connect(&tdApi,&TdApi::sendAllInstruments,this,&MainWindow::receiveAllInstruments);
    connect(&tdApi,&TdApi::sendTradingAccount,ui->fundTable,&FundTable::receiveTradingAccount);
    connect(&tdApi,&TdApi::sendInvestorPosition,ui->posTable,&PosTable::receiveInvestorPosition);
    connect(&tdApi,&TdApi::sendOrder,ui->entrustTable,&EntrustTable::receiveOrder);

    /* other */
    connect(&login,&Login::sendLoginField,this,&MainWindow::receiveLoginField);
    connect(this,&MainWindow::sendLog,this,&MainWindow::receiveLog);

    login.show();
}
void MainWindow::loginDone() {
    login.close();
    showMaximized();
    userStatus->setText("当前用户："+userInfo.UserID);
    iDebug<<"当前用户"<<userInfo.UserID<<"前置编号"<<userInfo.FrontID<<"会话编号"<<userInfo.SessionID;
    tdApi.reqQryTradingAccount();
    Sleep(1500);
    tdApi.reqQryInvestorPosition();
    Sleep(1500);
    tdApi.fetchAllInstruments();
}

void MainWindow::receiveError(QString msg)
{
    QMessageBox::critical(this,"错误",msg);
}
void MainWindow::receiveLoginField(LoginField data)
{
    mdApi.connect(data);
    tdApi.connect(data);
}

void MainWindow::receiveMdConnectionStatus(bool status)
{
    mdStatus->setColor(status?"green":"red");
}

void MainWindow::receiveTdConnectionStatus(bool status)
{
    tdStatus->setColor(status?"green":"red");
}

void MainWindow::receiveRspLoginMd(RspLoginField u)
{
    isMdLogin=true;
    if (isMdLogin&&isTdLogin) loginDone();
}
void MainWindow::receiveRspLoginTd(RspLoginField u)
{
    isTdLogin=true;
    userInfo=u;
    if (isMdLogin&&isTdLogin) loginDone();
}

void MainWindow::receiveAllInstruments(QVector<InstrumentField>instruments)
{
    mdApi.subscribe(instruments);
}

void MainWindow::receiveRtnDepthMarketData(QuoteField q)
{
    ui->quoteTable->updateQuote(q);
}

void MainWindow::receiveLog(QString msg)
{
    ui->logText->append(msg);
}

void MainWindow::on_action_triggered()
{
    tdApi.test1();
}

void MainWindow::on_action_2_triggered()
{
    tdApi.test2();
}
