#ifndef ENGINE_H
#define ENGINE_H

#include <queue>
#include <mutex>
#include <QThread>
#include "mdapi.h"
#include "tdapi.h"
#include "command.h"
#include "util.h"

class Engine:public QThread
{
    Q_OBJECT
public:
    Engine();
    ~Engine();
    void run() override; // 每秒运行一个tdApi
    void addCommand(std::shared_ptr<Command>); // 添加命令
    void release(); // 释放api
    void tradeInit(); // 结算结果确认
    void getAccountDetail(); // 依次请求用户资金信息、持仓信息、报单信息
    void getQuotes(); // 获取行情
	
public slots:
    void receiveLoginField(LoginField); // 登录按钮信号
	void receiveReqAuthenticateCommand(); // 添加请求认证命令的信号
	void receiveLoginCommand(); // 添加登录命令的信号
    void receiveAllInstruments(QVector<InstrumentField>); // 获取所有合约id回调
    void receiveReqOrderInsert(CThostFtdcInputOrderField); // 新建报单
public:
    MdApi mdApi; // 行情api
    TdApi tdApi; // 交易api
private:
    std::queue<std::shared_ptr<Command>>commandQueue;
    std::mutex queueMutex; // 队列锁
    bool working=true; // 通知工作线程
};

#endif // ENGINE_H
