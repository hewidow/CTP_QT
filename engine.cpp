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
                Command *command = commandQueue.first();
                result = command->execute();
                if (result==0) commandQueue.pop_front();
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
void Engine::addCommand(Command *c)
{
    std::lock_guard<std::mutex>lock(queueMutex);
    commandQueue.enqueue(c);
}
void Engine::release()
{
    mdApi.release();
    tdApi.release();
}
void Engine::receiveLoginField(LoginField data)
{
    mdApi.connect(data);
    tdApi.connect(data);
}
void Engine::tradeInit()
{
    addCommand(new ReqSettlementInfoConfirmCommand(&tdApi));
}

void Engine::getAccountDetail()
{
    addCommand(new ReqQryTradingAccountCommand(&tdApi));
    addCommand(new ReqQryInvestorPositionCommand(&tdApi));
    addCommand(new ReqQryOrderCommand(&tdApi));
}

void Engine::getQuotes()
{
    addCommand(new FetchAllInstrumentsCommand(&tdApi));
}

void Engine::receiveAllInstruments(QVector<InstrumentField>instruments)
{
    mdApi.subscribe(instruments);
}

void Engine::receiveReqOrderInsert(CThostFtdcInputOrderField t)
{
    tdApi.reqOrderInsert(t);
}
