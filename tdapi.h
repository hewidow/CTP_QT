#ifndef TDAPI_H
#define TDAPI_H

#include <QObject>
#include <QDir>
#include "api/ThostFtdcTraderApi.h"
#include "debug.h"
#include "util.h"
#include "global.h"

class TdApi : public QObject,public CThostFtdcTraderSpi
{
    Q_OBJECT
public:
    explicit TdApi(QObject *parent = nullptr);
    int release(); // 释放api
    int connect(LoginField); // 设置连接
    int reqAuthenticate(); // 客户端认证
    int login(); // 登录
    int reqSettlementInfoConfirm(); // 投资者结算结果确认，在开始每日交易前都需要先确认上一日结算单，只需要确认一次
    int reqQryTradingAccount(); // 请求查询资金账户
    int reqQryInvestorPosition(); // 请求查询投资者持仓
    int reqQryOrder(); // 请求查询报单
    int fetchAllInstruments(); // 查询正在交易的合约列表
    int reqOrderInsert(CThostFtdcInputOrderField); // 请求报单录入
    void test1();
    void test2();

private:
    void OnFrontConnected() override;
    void OnFrontDisconnected(int nReason) override;
    void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) override;
    void OnRtnOrder(CThostFtdcOrderField *pOrder) override;
    void OnRtnTrade(CThostFtdcTradeField *pTrade) override;

private:
    CThostFtdcTraderApi *api;
    int nRequestID=0;
    QVector<InstrumentField>instruments; // 所有合约
    QVector<CThostFtdcInvestorPositionField>positions; // 所有持仓
    QVector<CThostFtdcOrderField>orders; // 所有报单
    LoginField userInfo;

signals:
    void sendError(QString); // 发送错误信息
    void sendConnectionStatus(bool); // 发送连接状态
    void sendRspLogin(CThostFtdcRspUserLoginField); // 登录成功响应
    void sendAllInstruments(QVector<InstrumentField>); // 所有合约收集完成信号
    void sendTradingAccount(TradingAccount); // 发送账户资金信息
    void sendInvestorPositions(QVector<CThostFtdcInvestorPositionField>); // 发送投资者持仓
    void sendOrders(QVector<CThostFtdcOrderField>); // 发送报单信息
};

#endif // TDAPI_H
