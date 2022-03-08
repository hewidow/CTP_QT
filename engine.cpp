#include "engine.h"

Engine::Engine()
{

}

Engine::~Engine()
{
    working=false;
    quit();
    wait();
}
void Engine::run(){
    while (working){
        if (commandQueue.size()){
            int result = INT_MAX;
            {
                std::lock_guard<std::mutex>lock(queueMutex);
                std::shared_ptr<Command>command = commandQueue.front();
                result = command->execute(&tdApi);
                if (result==0) commandQueue.pop();
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
void Engine::addCommand(std::shared_ptr<Command> newCommand)
{
    std::lock_guard<std::mutex>lock(queueMutex);
    commandQueue.push(newCommand);
}
void Engine::release()
{
    mdApi.release();
    addCommand(std::make_shared<ReleaseCommand>());
}
void Engine::receiveLoginField(LoginField data)
{
    mdApi.connect(data);
    addCommand(std::make_shared<ConnectCommand>(data));
}
void Engine::tradeInit()
{
    addCommand(std::make_shared<ReqSettlementInfoConfirmCommand>());
}

void Engine::getAccountDetail()
{
    addCommand(std::make_shared<ReqQryTradingAccountCommand>());
    addCommand(std::make_shared<ReqQryInvestorPositionCommand>());
    addCommand(std::make_shared<ReqQryOrderCommand>());
}

void Engine::getQuotes()
{
    addCommand(std::make_shared<FetchAllInstrumentsCommand>());
}

void Engine::receiveAllInstruments(QVector<InstrumentField>instruments)
{
    mdApi.subscribe(instruments);
}

void Engine::receiveReqOrderInsert(CThostFtdcInputOrderField t)
{
	addCommand(std::make_shared<ReqOrderInsertCommand>(t));
}
