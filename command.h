#ifndef COMMAND_H
#define COMMAND_H

#include "tdapi.h"

// 命令模板
class Command {
public:
	Command() {};
	~Command() {};
	virtual QString name() = 0;
	virtual int execute(TdApi *tdApi) = 0;
};
// 设置ReleaseCommand和ConnectCommand是为了让所有的tdApi都走commandQueue，省去引入锁
class ReleaseCommand : public Command {
public:
	ReleaseCommand() {};
	QString name() { return "请求释放交易API"; };
	int execute(TdApi *tdApi) {
		return tdApi->release();
	};
};
class ConnectCommand :public Command {
public:
	ConnectCommand(LoginField _data) :Command(), data(_data) {};
	QString name() { return "请求交易连接"; };
	int execute(TdApi *tdApi) {
		return tdApi->connect(data);
	};
private:
	LoginField data;
};
class ReqAuthenticateCommand :public Command {
public:
	ReqAuthenticateCommand(){};
	QString name() { return "请求客户端认证"; };
	int execute(TdApi *tdApi) {
		return tdApi->reqAuthenticate();
	};
};
class LoginCommand :public Command {
public:
	LoginCommand() {};
	QString name() { return "请求交易登录"; };
	int execute(TdApi *tdApi) {
		return tdApi->login();
	};
};
class ReqSettlementInfoConfirmCommand :public Command {
public:
	ReqSettlementInfoConfirmCommand() {};
	QString name() { return "请求投资者结算结果确认"; };
	int execute(TdApi *tdApi) {
		return tdApi->reqSettlementInfoConfirm();
	};
};

class ReqQryTradingAccountCommand :public Command {
public:
	ReqQryTradingAccountCommand() {};
	QString name() { return "请求查询资金账户"; };
	int execute(TdApi *tdApi) {
		return tdApi->reqQryTradingAccount();
	};
};

class ReqQryInvestorPositionCommand :public Command {
public:
	ReqQryInvestorPositionCommand() {};
	QString name() { return "请求查询投资者持仓"; };
	int execute(TdApi *tdApi) {
		return tdApi->reqQryInvestorPosition();
	};
};

class ReqQryOrderCommand :public Command {
public:
	ReqQryOrderCommand() {};
	QString name() { return "请求查询报单"; };
	int execute(TdApi *tdApi) {
		return tdApi->reqQryOrder();
	};
};
class ReqAllInstrumentsCommand :public Command {
public:
	ReqAllInstrumentsCommand() {};
	QString name() { return "请求查询所有合约"; };
	int execute(TdApi *tdApi) {
		return tdApi->reqAllInstruments();
	};
};
class ReqOrderInsertCommand :public Command {
public:
	ReqOrderInsertCommand(CThostFtdcInputOrderField _t) :Command(), t(_t) {};
	QString name() { return "请求报单录入"; };
	int execute(TdApi *tdApi) {
		return tdApi->reqOrderInsert(t);
	};
private:
	CThostFtdcInputOrderField t;
};

class ReqOrderActionCommand :public Command {
public:
    ReqOrderActionCommand(CThostFtdcInputOrderActionField _t) :Command(), t(_t) {};
    QString name() { return "请求报单操作"; };
    int execute(TdApi *tdApi) {
        return tdApi->reqOrderAction(t);
    };
private:
    CThostFtdcInputOrderActionField t;
};
#endif // COMMAND_H
