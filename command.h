#ifndef COMMAND_H
#define COMMAND_H

#include "tdapi.h"

// 命令模板
class Command {
public:
    Command(TdApi *_tdApi):tdApi(_tdApi){};
    ~Command(){tdApi=nullptr;};
    virtual int execute()=0;
protected:
    TdApi *tdApi;
};

class ReqSettlementInfoConfirmCommand:public Command{
public:
    ReqSettlementInfoConfirmCommand(TdApi *_tdApi):Command(_tdApi){};
    int execute() {
        return tdApi->reqSettlementInfoConfirm();
    };
};

class ReqQryTradingAccountCommand:public Command{
public:
    ReqQryTradingAccountCommand(TdApi *_tdApi):Command(_tdApi){};
    int execute() {
        return tdApi->reqQryTradingAccount();
    };
};

class ReqQryInvestorPositionCommand:public Command{
public:
    ReqQryInvestorPositionCommand(TdApi *_tdApi):Command(_tdApi){};
    int execute() {
        return tdApi->reqQryInvestorPosition();
    };
};

class ReqQryOrderCommand:public Command{
public:
    ReqQryOrderCommand(TdApi *_tdApi):Command(_tdApi){};
    int execute() {
        return tdApi->reqQryOrder();
    };
};
class FetchAllInstrumentsCommand:public Command{
public:
    FetchAllInstrumentsCommand(TdApi *_tdApi):Command(_tdApi){};
    int execute() {
        return tdApi->fetchAllInstruments();
    };
};
#endif // COMMAND_H
