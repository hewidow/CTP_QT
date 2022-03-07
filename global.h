﻿#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include "ThostFtdcUserApiStruct.h"

#define SUBSCRIBE_INFO_PATH QString("subscribeInfo/") // 订阅信息存放地址
#define CONFIG_PATH QString("config.ini") // 一些用户配置选项

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
