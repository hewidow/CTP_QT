#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

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
struct RspLoginField // 登录响应表单
{
    ///交易日
    QString TradingDay;
    ///登录成功时间
    QString LoginTime;
    ///经纪公司代码
    QString BrokerID;
    ///用户代码
    QString UserID;
    ///交易系统名称
    QString SystemName;
    ///前置编号
    int FrontID;
    ///会话编号
    int SessionID;
    ///最大报单引用
    QString MaxOrderRef;
    ///上期所时间
    QString SHFETime;
    ///大商所时间
    QString DCETime;
    ///郑商所时间
    QString CZCETime;
    ///中金所时间
    QString FFEXTime;
    ///能源中心时间
    QString INETime;
};
struct TradingAccount{ // 账户资金信息
    double Available;
    double FrozenCash;
    double WithdrawQuota;
};
struct InvestorPosition{ // 投资者持仓
    QString InstrumentID;
    int Position;
    double PositionProfit;
};
struct Order{ // 报单
    QString OrderSysID;
    QString InstrumentID;
    char Direction; // 买0卖1
    double LimitPrice;
    int VolumeTotalOriginal;
    int VolumeTotal;
//    ///全部成交
//    #define THOST_FTDC_OST_AllTraded '0'
//    ///部分成交还在队列中
//    #define THOST_FTDC_OST_PartTradedQueueing '1'
//    ///部分成交不在队列中
//    #define THOST_FTDC_OST_PartTradedNotQueueing '2'
//    ///未成交还在队列中
//    #define THOST_FTDC_OST_NoTradeQueueing '3'
//    ///未成交不在队列中
//    #define THOST_FTDC_OST_NoTradeNotQueueing '4'
//    ///撤单
//    #define THOST_FTDC_OST_Canceled '5'
//    ///未知
//    #define THOST_FTDC_OST_Unknown 'a'
//    ///尚未触发
//    #define THOST_FTDC_OST_NotTouched 'b'
//    ///已触发
//    #define THOST_FTDC_OST_Touched 'c'
    char OrderStatus;
    QString InsertTime;
};

struct InstrumentField // 合约信息
{
    QString InstrumentID;
    QString InstrumentName;
};

struct QuoteField // 行情响应表单
{
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
