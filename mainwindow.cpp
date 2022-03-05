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
    qRegisterMetaType<RspLoginField>("RspLoginField");
    qRegisterMetaType<QuoteField>("QuoteField");
    qRegisterMetaType<QVector<InstrumentField>>("QVector<InstrumentField>");
    connect(&mdApi,&MdApi::sendError,this,&MainWindow::receiveError);
    connect(&mdApi,&MdApi::sendConnectionStatus,this,&MainWindow::receiveMdConnectionStatus);
    connect(&mdApi,&MdApi::sendRspLogin,this,&MainWindow::receiveRspLoginMd);
    connect(&mdApi,&MdApi::sendRtnDepthMarketData,this,&MainWindow::receiveRtnDepthMarketData);

    connect(&tdApi,&TdApi::sendError,this,&MainWindow::receiveError);
    connect(&tdApi,&TdApi::sendConnectionStatus,this,&MainWindow::receiveTdConnectionStatus);
    connect(&tdApi,&TdApi::sendRspLogin,this,&MainWindow::receiveRspLoginTd);
    connect(&tdApi,&TdApi::sendAllInstruments,this,&MainWindow::receiveAllInstruments);

    connect(&login,&Login::sendLoginField,this,&MainWindow::receiveLoginField);

    connect(this,&MainWindow::sendLog,this,&MainWindow::receiveLog);
    login.show();
}
void MainWindow::loginDone() {
    login.close();
    showMaximized();
    userStatus->setText("当前用户："+userInfo.UserID);
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
    iDebug<<"test";
    iDebug<<"test";
    iDebug<<"test";
    iDebug<<"test";
    iDebug<<"test";
    iDebug<<"test";
    iDebug<<"test";
    iDebug<<"test";
    iDebug<<"test";
    iDebug<<"test";
    iDebug<<"test";
    iDebug<<"test";
    iDebug<<"test";
    iDebug<<"test";
    iDebug<<"test";
}

void MainWindow::on_action_2_triggered()
{
    auto rInt=[]()->int{
        return qrand()-RAND_MAX/2;
    };
    auto rDouble=[]()->double{
        return qrand()-RAND_MAX/2;
    };
    ui->quoteTable->updateQuote(QuoteField{
        "测试",
        "test",
        "Test",
        rDouble(),
        rDouble(),
        rDouble(),
        rDouble(),
        rInt(),
        rDouble(),
        rInt(),
        rInt(),
        rDouble(),
        rDouble(),
        rDouble()
    });
}
