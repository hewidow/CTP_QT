#ifndef COMMAND_H
#define COMMAND_H

#include "tdapi.h"

// 命令模板
class Command {
public:
    Command(){};
    ~Command(){};
    virtual int execute(TdApi *tdApi)=0;
};
// 设置ReleaseCommand和ConnectCommand是为了让所有的tdApi都走commandQueue，省去引入锁
class ReleaseCommand: public Command {
public:
    ReleaseCommand(){};
    int execute(TdApi *tdApi) {
        return tdApi->release();
    };
};
class ConnectCommand:public Command{
public:
    ConnectCommand(LoginField _data):Command(),data(_data){};
    int execute(TdApi *tdApi) {
        return tdApi->connect(data);
    };
private:
    LoginField data;
};

class ReqSettlementInfoConfirmCommand:public Command{
public:
    ReqSettlementInfoConfirmCommand(){};
    int execute(TdApi *tdApi) {
        return tdApi->reqSettlementInfoConfirm();
    };
};

class ReqQryTradingAccountCommand:public Command{
public:
    ReqQryTradingAccountCommand(){};
    int execute(TdApi *tdApi) {
        return tdApi->reqQryTradingAccount();
    };
};

class ReqQryInvestorPositionCommand:public Command{
public:
    ReqQryInvestorPositionCommand(){};
    int execute(TdApi *tdApi) {
        return tdApi->reqQryInvestorPosition();
    };
};

class ReqQryOrderCommand:public Command{
public:
    ReqQryOrderCommand(){};
    int execute(TdApi *tdApi) {
        return tdApi->reqQryOrder();
    };
};
class FetchAllInstrumentsCommand:public Command{
public:
    FetchAllInstrumentsCommand(){};
    int execute(TdApi *tdApi) {
        return tdApi->fetchAllInstruments();
    };
};
#endif // COMMAND_H
