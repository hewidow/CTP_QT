#ifndef CTPAPI_H
#define CTPAPI_H

#include <QObject>
#include <QDir>
#include "api/ThostFtdcMdApi.h"
#include "debug.h"
#include "util.h"


class MdApi : public QObject,public CThostFtdcMdSpi
{
    Q_OBJECT
public:
    explicit MdApi(QObject *parent = nullptr);
    // 释放api
    void release();
    // 设置连接
    void connect(LoginForm);
    // 登录（行情登录不做校验）
    void login();
    // 订阅合约
    void subscribe(QVector<InstrumentField>);

private:
    void OnFrontConnected() override;
    void OnFrontDisconnected(int) override;
    void OnRspUserLogin(CThostFtdcRspUserLoginField *, CThostFtdcRspInfoField *, int, bool) override;
    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *) override;

private:
    CThostFtdcMdApi *api;
    int nRequestID=0;
    QMap<QString,InstrumentField>instrumentsMap; // 合约id到合约的映射

signals:
    // 发送错误信息
    void sendError(QString);
    // 发送连接状态
    void sendConnectionStatus(bool);
    // 登录成功响应
    void sendRspLogin(CThostFtdcRspUserLoginField);
    // 订阅合约响应
    void sendRtnDepthMarketData(QuoteField);
};

#endif // CTPAPI_H
