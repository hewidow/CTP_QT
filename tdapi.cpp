#include "tdapi.h"

TdApi::TdApi(QObject *parent) : QObject(parent)
{
   api=nullptr;
}
int TdApi::release()
{
    if (api!=nullptr) {
        iDebug<<"释放交易api";
        api->RegisterSpi(NULL);
        api->Release();
        api=nullptr;
    }
    return 0; // 本地操作，api总是执行成功
}
int TdApi::connect(LoginField u)
{
    QDir dir;
    if (!dir.exists(SUBSCRIBE_INFO_PATH)) dir.mkpath(SUBSCRIBE_INFO_PATH);

    api=CThostFtdcTraderApi::CreateFtdcTraderApi(SUBSCRIBE_INFO_PATH.toStdString().c_str()); // 初始化api
    api->RegisterSpi(this); // 绑定回调接口
    api->SubscribePublicTopic(THOST_TERT_QUICK);
    api->SubscribePrivateTopic(THOST_TERT_QUICK);
    api->RegisterFront(Util::convertQStringToCharPoint(u.tdAddress)); // 设置连接地址
    api->Init(); // 初始化运行环境
    userInfo=u; // 复制登录账户信息
    iDebug<<"交易地址"<<u.tdAddress;
    return 0; // 本地操作，api总是执行成功
}
void TdApi::OnFrontConnected()
{
    iDebug<<"交易连接成功";
    emit sendConnectionStatus(true);
    reqAuthenticate();
}
void TdApi::OnFrontDisconnected(int nReason)
{
    iDebug<<"交易连接断开"<<QString::number(nReason,16);
    emit sendConnectionStatus(false);
}

void TdApi::reqAuthenticate()
{
    CThostFtdcReqAuthenticateField a = {{0}};
    strcpy_s(a.BrokerID, userInfo.brokerId.toStdString().c_str());
    strcpy_s(a.UserID, userInfo.userId.toStdString().c_str());
    strcpy_s(a.AuthCode, userInfo.authCode.toStdString().c_str());
    strcpy_s(a.AppID, userInfo.appId.toStdString().c_str());
    int code=api->ReqAuthenticate(&a, ++nRequestID);
    iDebug<<"请求客户端认证"<<Util::convertApiReturnValueToText(code);
}

void TdApi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    rDebug("认证响应",pRspInfo);
    if (pRspInfo->ErrorID==0) login();
    else emit sendError("交易认证失败");
}

void TdApi::login()
{
    CThostFtdcReqUserLoginField t={{0}};
    strcpy_s(t.BrokerID,userInfo.brokerId.toStdString().c_str());
    strcpy_s(t.UserID,userInfo.userId.toStdString().c_str());
    strcpy_s(t.Password,userInfo.password.toStdString().c_str());
    int code=api->ReqUserLogin(&t,++nRequestID);
    iDebug<<"请求登录"<<Util::convertApiReturnValueToText(code);
}
void TdApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    rDebug("交易登录响应",pRspInfo);
    if (pRspInfo->ErrorID==0) {
        emit sendRspLogin(*pRspUserLogin);
    }
    else emit sendError("交易登录失败");
}
int TdApi::reqSettlementInfoConfirm()
{
    CThostFtdcSettlementInfoConfirmField Confirm = { {0} };
    strcpy_s(Confirm.BrokerID, userInfo.brokerId.toStdString().c_str());
    strcpy_s(Confirm.InvestorID, userInfo.userId.toStdString().c_str());
    int code=api->ReqSettlementInfoConfirm(&Confirm, nRequestID++);
    iDebug<<"请求投资者结算结果确认"<<Util::convertApiReturnValueToText(code);
    return code;
}
void TdApi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    rDebug("投资者结算结果确认响应",pRspInfo);
}
int TdApi::reqQryTradingAccount()
{
    CThostFtdcQryTradingAccountField a = {{0}};
    strcpy_s(a.BrokerID, userInfo.brokerId.toStdString().c_str());
    strcpy_s(a.InvestorID, userInfo.userId.toStdString().c_str());
    strcpy_s(a.CurrencyID, "CNY");
    int code=api->ReqQryTradingAccount(&a,++nRequestID);
    iDebug<<"请求查询资金账户"<<Util::convertApiReturnValueToText(code);
    return code;
}
void TdApi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    rDebug("查询资金账户响应",pRspInfo); // pRspInfo为nullptr，很奇怪
    emit sendTradingAccount(TradingAccount{
        pTradingAccount->Available,
        pTradingAccount->FrozenCash,
        pTradingAccount->WithdrawQuota
    });
}
int TdApi::reqQryInvestorPosition()
{
    positions.clear(); // 清空持仓记录的容器
    CThostFtdcQryInvestorPositionField a = {{0}};
    strcpy_s(a.BrokerID, userInfo.brokerId.toStdString().c_str());
    strcpy_s(a.InvestorID, userInfo.userId.toStdString().c_str());
    int code=api->ReqQryInvestorPosition(&a, ++nRequestID);
    iDebug<<"请求查询投资者持仓"<<Util::convertApiReturnValueToText(code);
    return code;
}

void TdApi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pInvestorPosition!=nullptr) positions.push_back(*pInvestorPosition);
    if (bIsLast) {
        rDebug("查询投资者持仓响应",pRspInfo);
        iDebug<<"持仓总数"<<positions.size();
        emit sendInvestorPositions(positions);
    }
}
int TdApi::reqQryOrder()
{
    orders.clear(); // 清空报单记录的容器
    CThostFtdcQryOrderField a = { {0} };
    strcpy_s(a.BrokerID, userInfo.brokerId.toStdString().c_str());
    strcpy_s(a.InvestorID, userInfo.userId.toStdString().c_str());
    int code=api->ReqQryOrder(&a, ++nRequestID);
    iDebug<<"请求查询报单"<<Util::convertApiReturnValueToText(code);
    return code;
}

void TdApi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pOrder!=nullptr) orders.push_back(*pOrder);
    if (bIsLast) {
        rDebug("查询报单响应",pRspInfo);
        iDebug<<"报单总数"<<orders.size();
        emit sendOrders(orders);
    }
}
int TdApi::fetchAllInstruments()
{
    instruments.clear(); // 清空合约记录的容器
    CThostFtdcQryInstrumentField a = {{0}};
    int code=api->ReqQryInstrument(&a,++nRequestID);
    iDebug<<"请求查询所有合约"<<Util::convertApiReturnValueToText(code);
    return code;
}
void TdApi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    // 只要长度6位且后四位为如下条件的，这样少一点
    if (strlen(pInstrument->InstrumentID)==6&&strcmp(&(pInstrument->InstrumentID[2]),"2203")==0) {
        instruments.push_back(InstrumentField{
            pInstrument->InstrumentID,
            QString::fromLocal8Bit(pInstrument->InstrumentName),
            pInstrument->ExchangeID
        });
    }
    if (bIsLast) {
        rDebug("查询所有合约响应",pRspInfo);
        emit sendAllInstruments(instruments);
    }
}
void TdApi::reqOrderInsert(CThostFtdcInputOrderField t)
{
   int code=api->ReqOrderInsert(&t,++nRequestID);
   iDebug<<"请求报单录入"<<Util::convertApiReturnValueToText(code);
}
void TdApi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    rDebug("OnRspOrderInsert字段填写不对",pRspInfo);
}

void TdApi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
    rDebug("OnErrRtnOrderInsert字段填写不对",pRspInfo);
}

void TdApi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
    iDebug<<"OnRtnOrder";
}

void TdApi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
    iDebug<<"OnRtnTrade成交通知";
}
void TdApi::test1(){

}
void TdApi::test2(){

    CThostFtdcOrderField t={{0}};
    strcpy_s(t.OrderSysID,QString("test"+QString::number(qrand())).toStdString().c_str());
    strcpy_s(t.InstrumentID,"Test");
    t.Direction =  char((int)'0'+qrand()%2);
    t.LimitPrice = 1234.56;
    t.VolumeTotalOriginal = 1234;
    t.VolumeTotal = 5678;
    t.OrderStatus = '0';
    strcpy_s(t.InsertTime,"12:00:01");
    emit sendOrders({t});
}
