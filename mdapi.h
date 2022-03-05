#ifndef CTPAPI_H
#define CTPAPI_H

#include <QObject>
#include <QDir>
#include "global.h"
#include "api/ThostFtdcMdApi.h"
#include "debug.h"
#include "util.h"


class MdApi : public QObject,public CThostFtdcMdSpi
{
    Q_OBJECT
public:
    explicit MdApi(QObject *parent = nullptr);
    void connect(LoginField); // 设置连接
    void release(); // 释放连接
    void login(); // 登录（行情登录不做校验）
    void subscribe(QVector<InstrumentField>); // 订阅合约

private:
    void OnFrontConnected() override;
    void OnFrontDisconnected(int) override;
    void OnRspUserLogin(CThostFtdcRspUserLoginField *, CThostFtdcRspInfoField *, int, bool) override;
    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *) override;

private:
    CThostFtdcMdApi *api;
    int nRequestID=0;
    QMap<QString,QString>instrumentIdToName;

signals:
    void sendError(QString); // 发送错误信息
    void sendConnectionStatus(bool); // 发送连接状态
    void sendRspLogin(RspLoginField); // 登录成功响应
    void sendRtnDepthMarketData(QuoteField); // 订阅合约响应
};

#endif // CTPAPI_H
