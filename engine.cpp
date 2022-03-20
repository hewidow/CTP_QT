#include "engine.h"

Engine::Engine()
{
	start();
}

Engine::~Engine()
{
    working=false;
    quit();
    wait();
}
void Engine::run(){
    int retry = 0;
    int count = 0;
    while (working){
        { // 加锁
            std::lock_guard<std::mutex>lock(queueMutex);
            if (commandQueue.size()) {
                int result = INT_MAX;
                std::shared_ptr<Command>command = commandQueue.front();
                result = command->execute(&tdApi);
                iDebug << command->name() << "→" << Util::convertApiReturnValueToText(result);
                ++retry;
                if (result == 0 || retry > COMMAND_QUEUE_RETRY_COUNT) {
                    commandQueue.pop_front();
                    if (retry > COMMAND_QUEUE_RETRY_COUNT) emit sendError("超出请求重试次数");
                    retry = 0;
                    if (command->name() == "请求查询报单") accountDetailCommandsInQueue = false; // 暂时使用这种方法完成需求
                }
            }
        } // 释放锁
        std::this_thread::sleep_for(std::chrono::seconds(COMMAND_QUEUE_REFRESH_INTERVAL));
        // 定时获取一次用户账户信息
        if (++count == ACCOUNT_DETAIL_REFRESH_INTERVAL) {
            count = 0;
            if (initDone) getAccountDetail(); // 初始化完成才开始获取用户信息
        }
        // 连接前置超时逻辑
        if (connectTimeOut >= 0) ++connectTimeOut;
        if (connectTimeOut > CONNECT_TIME_OUT) {
            release();
            emit sendError("连接前置超时");
        }
    }
}
void Engine::addCommand(std::shared_ptr<Command> newCommand,bool back=true)
{
    std::lock_guard<std::mutex>lock(queueMutex);
    if (back) commandQueue.push_back(newCommand);
    else commandQueue.push_front(newCommand); // 从前面插入，提高命令的优先级
}
void Engine::clearCommand()
{
    std::lock_guard<std::mutex>lock(queueMutex);
    while (commandQueue.size()) commandQueue.pop_front();
}
void Engine::release()
{
    mdApi.release();
    clearCommand();
    addCommand(std::make_shared<ReleaseCommand>());
    initDone = false; // 重置变量
    connectTimeOut = CONNECT_NOT;
}
void Engine::receiveLoginForm(LoginForm data)
{
    if (connectTimeOut == CONNECT_NOT) { // 避免重复收到登录事件
        mdApi.connect(data);
        connectTimeOut = 0; // 开始计时
        addCommand(std::make_shared<ConnectCommand>(data));
    }
}
void Engine::receiveReqAuthenticateCommand()
{
    connectTimeOut = CONNECT_DONE; // 连接前置成功，则不需要计时
	addCommand(std::make_shared<ReqAuthenticateCommand>());
}

void Engine::receiveLoginCommand()
{
	addCommand(std::make_shared<LoginCommand>());
}

void Engine::tradeInit()
{
    addCommand(std::make_shared<ReqSettlementInfoConfirmCommand>());
}

void Engine::getAccountDetail()
{
    if (!accountDetailCommandsInQueue) {
        accountDetailCommandsInQueue = true;
        addCommand(std::make_shared<ReqQryTradingAccountCommand>());
        addCommand(std::make_shared<ReqQryInvestorPositionCommand>());
        addCommand(std::make_shared<ReqQryOrderCommand>());
    }
}

void Engine::getQuotes()
{
    addCommand(std::make_shared<ReqAllInstrumentsCommand>());
}

void Engine::receiveAllInstruments(QVector<InstrumentField>instruments)
{
    for (auto& it : instruments) {
        instrumentsMap[it.InstrumentID] = it;
    }
    mdApi.subscribe(instruments);
    initDone = true;
}

void Engine::receiveReqOrderInsert(CThostFtdcInputOrderField t)
{
    strcpy_s(t.BrokerID, userInfo.BrokerID);
    strcpy_s(t.InvestorID, userInfo.UserID);
    strcpy_s(t.UserID, userInfo.UserID);
    strcpy_s(t.ExchangeID, instrumentsMap[t.InstrumentID].ExchangeID.toStdString().c_str());
    addCommand(std::make_shared<ReqOrderInsertCommand>(t),false);
}

void Engine::receiveReqOrderAction(CThostFtdcInputOrderActionField t)
{
    strcpy_s(t.BrokerID, userInfo.BrokerID);
    strcpy_s(t.InvestorID, userInfo.UserID);
    strcpy_s(t.UserID, userInfo.UserID);
    strcpy_s(t.ExchangeID, instrumentsMap[t.InstrumentID].ExchangeID.toStdString().c_str());
    addCommand(std::make_shared<ReqOrderActionCommand>(t),false);
}

void Engine::receiveOrderChange()
{
    getAccountDetail();
}

//void Engine::syncGetUserInfo()
//{
//    addCommand(std::make_shared<ReqQryTradingAccountCommand>());
//    while (true) {
//
//        std::this_thread::sleep_for(std::chrono::seconds(SYNC_COMMAND_REFRESH_INTERVAL));
//    }
//}
