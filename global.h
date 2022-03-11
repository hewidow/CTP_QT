﻿#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include "ThostFtdcUserApiStruct.h"

#define SUBSCRIBE_INFO_PATH QString("subscribeInfo/") // 订阅信息存放地址
#define CONFIG_PATH QString("config.ini") // 一些用户配置选项
#define COMMAND_QUEUE_RETRY_COUNT 3 // 命令队列重试次数
#define COMMAND_QUEUE_REFRESH_INTERVAL 1 // 命令队列刷新间隔（秒）
#define ACCOUNT_DETAIL_REFRESH_INTERVAL 10 // 账户详情刷新间隔（数字*COMMAND_QUEUE_REFRESH_INTERVAL）
#define CONNECT_TIME_OUT 3 // 连接前置超时时间（数字*COMMAND_QUEUE_REFRESH_INTERVAL）
#define STRATEGY_PATH QString("strategy/") // 策略存放地址
#define STRATEGY_MODEL_RUN_INTERVAL 1000 // 策略模型运行间隔（毫秒）

struct LoginField // 登录表单
{
    QString mdAddress; // 行情地址
    QString tdAddress; // 交易地址
    QString brokerId; // BrokerID
    QString appId; // 产品名称
    QString authCode; // 授权编码
    QString userId; // 账户
    QString password; // 密码
};
struct TradingAccount{ // 账户资金信息
    double Available;
    double FrozenCash;
    double WithdrawQuota;
};

struct InstrumentField // 合约信息
{
    QString InstrumentID;
    QString InstrumentName;
    QString ExchangeID;
};

struct QuoteField // 行情响应表单
{
    QString ExchangeID;
    QString InstrumentID;
    QString InstrumentName;
    QString UpdateTime;
    double LastPrice;
    double UpDown; // 涨跌
    double Gain; // 幅度
    double BidPrice1;
    int BidVolume1;
    double AskPrice1;
    int AskVolume1;
    int Volume; // 成交量
    double OpenPrice;
    double HighestPrice;
    double LowestPrice;
};

#endif // GLOBAL_H
