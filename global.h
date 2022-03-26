#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QDate>
#include "ThostFtdcUserApiStruct.h"

#define INSTRUMENT_ID_FILTER "204"
#define SUBSCRIBE_INFO_PATH "subscribeInfo/" // 订阅信息存放地址
#define CONFIG_PATH "config.ini" // 一些用户配置选项
#define COMMAND_QUEUE_RETRY_COUNT 3 // 命令队列重试次数
#define COMMAND_QUEUE_REFRESH_INTERVAL 1 // 命令队列刷新间隔（秒）
#define ACCOUNT_DETAIL_REFRESH_INTERVAL 10 // 账户详情刷新间隔（数字*COMMAND_QUEUE_REFRESH_INTERVAL）
#define CONNECT_TIME_OUT 3 // 连接前置超时时间（数字*COMMAND_QUEUE_REFRESH_INTERVAL）
#define STRATEGY_PATH "strategy/" // 策略存放地址
#define STRATEGY_MODEL_RUN_INTERVAL 5000 // 策略模型运行间隔（毫秒）
#define SYNC_COMMAND_REFRESH_INTERVAL 1 // 同步命令刷新间隔（秒）
#define TICK_HISTORY_LENGTH 50 // 每个合约的历史tick最大长度
#define LOG_MAX_ROW_COUNT 50 // 记录log的最大行数
#define DATABASE_PATH "database/" // 数据存放地址

struct LoginForm // 登录表单
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
    double total;
    double FrozenMargin;
    double Available;
    double PositionProfit;
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
struct KLine // k线
{
    QString InstrumentID;
    QString ExchangeID;
    QDate dateTime;
    double volume;
    double turnover;
    double openInterest;
    double openPrice; // 一分钟开始价
    double highPrice; // 一分钟中的最高价
    double lowPrice; // 一分钟中的最低价
    double closePrice; // 一分钟结束价
};
struct BacktestForm
{
    QString databaseName;
    QDate startTime;
    QDate endTime;
    double startFund;
};
struct BacktestResult
{
    QDate startTime;
    QDate endTime;
    int totalTradingDays;
    int profitableTradingDays;
    int lossTradingDays;
    double startFund;
    double endFund;
    double profitRate;
    double annualizedProfit;
    double maximumDrawdown;
    double maximumDrawdownPercent;
    double totalProfitAndLoss;
    double totalHandlingFee;
    double totalTurnover;
    int totalTransactionNum;
};
#endif // GLOBAL_H
