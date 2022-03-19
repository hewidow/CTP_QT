#ifndef ENGINE_H
#define ENGINE_H

#include <deque>
#include <mutex>
#include <QThread>
#include "mdapi.h"
#include "tdapi.h"
#include "command.h"
#include "util.h"

#define CONNECT_NOT -1 // 未连接前置
#define CONNECT_DONE -2 // 已连接前置

class Engine:public QThread
{
    Q_OBJECT
public:
    Engine();
    ~Engine();
    // 启动子线程，每秒运行一个命令
    void run() override;
    // 添加命令
    void addCommand(std::shared_ptr<Command>,bool);
    // 清空所有命令
    void clearCommand();
    // 释放api
    void release();
    // 结算结果确认
    void tradeInit();
    // 依次请求用户资金信息、持仓信息、报单信息
    void getAccountDetail();
    // 获取行情
    void getQuotes();
	
public slots:
    // 登录按钮信号
    void receiveLoginField(LoginField);
    // 添加请求认证命令的信号
	void receiveReqAuthenticateCommand();
    // 添加登录命令的信号
	void receiveLoginCommand();
    // 获取所有合约id回调
    void receiveAllInstruments(QVector<InstrumentField>);
    // 新建报单
    void receiveReqOrderInsert(CThostFtdcInputOrderField);
    // 修改报单
    void receiveReqOrderAction(CThostFtdcInputOrderActionField);
    // 报单变更事件
    void receiveOrderChange();
signals:
    // 发送错误信息
    void sendError(QString);
public:
    MdApi mdApi; // 行情api
    TdApi tdApi; // 交易api
    CThostFtdcRspUserLoginField userInfo; // 用户信息
    QMap<QString, InstrumentField>instrumentsMap;
private:
    std::deque<std::shared_ptr<Command>>commandQueue;
    std::mutex queueMutex; // 队列锁
    bool working=true; // 子线程是否运行
    bool initDone = false; // 初始化工作完成情况
    int connectTimeOut = CONNECT_NOT; // 连接前置超时时间计算
    bool accountDetailCommandsInQueue = false; // getAccountDetail的相关命令是否在队列里，防止查询过多
};

#endif // ENGINE_H
