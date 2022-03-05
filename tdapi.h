#ifndef TDAPI_H
#define TDAPI_H

#include <QObject>
#include <QDir>
#include <windows.h>
#include "debug.h"
#include "util.h"
#include "global.h"
#include "api/ThostFtdcTraderApi.h"

class TdApi : public QObject,public CThostFtdcTraderSpi
{
    Q_OBJECT
public:
    explicit TdApi(QObject *parent = nullptr);
    void release(); // 释放api
    void connect(LoginField); // 设置连接
    void reqAuthenticate(); // 客户端认证
    void login(); // 登录
    void fetchAllInstruments(); // 查询正在交易的合约列表
    void reqQryTradingAccount(); // 请求查询资金账户
    void reqQryInvestorPosition(); // 请求查询投资者持仓
    void reqOrderInsert(); // 请求报单录入
    void test1();
    void test2();

private:
    void OnFrontConnected() override;
    void OnFrontDisconnected(int nReason) override;
    void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

private:
    CThostFtdcTraderApi *api;
    int nRequestID=0;
    QVector<InstrumentField> instruments; // 所有合约
    LoginField userInfo;

signals:
    void sendError(QString); // 发送错误信息
    void sendConnectionStatus(bool); // 发送连接状态
    void sendRspLogin(RspLoginField); // 登录成功响应
    void sendAllInstruments(QVector<InstrumentField>); // 所有合约收集完成信号
    void sendTradingAccount(TradingAccount); // 发送账户资金信息
    void sendInvestorPosition(InvestorPosition); // 发送投资者持仓
    void sendOrder(Order); // 发送报单信息
};

#endif // TDAPI_H
