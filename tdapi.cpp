#include "tdapi.h"

TdApi::TdApi(QObject *parent) : QObject(parent)
{
   api=nullptr;
}
void TdApi::release()
{
    if (api!=nullptr) {
        iDebug<<"释放交易api";
        api->Release();
    }
}
void TdApi::connect(LoginField u)
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
    rDebug("认证响应",pRspInfo->ErrorMsg);
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
    rDebug("交易登录响应",pRspInfo->ErrorMsg);
    if (pRspInfo->ErrorID==0) {
        emit sendRspLogin(RspLoginField{
          pRspUserLogin->TradingDay,
          pRspUserLogin->LoginTime,
          pRspUserLogin->BrokerID,
          pRspUserLogin->UserID,
          pRspUserLogin->SystemName,
          pRspUserLogin->FrontID,
          pRspUserLogin->SessionID,
          pRspUserLogin->MaxOrderRef,
          pRspUserLogin->SHFETime,
          pRspUserLogin->DCETime,
          pRspUserLogin->CZCETime,
          pRspUserLogin->FFEXTime,
          pRspUserLogin->INETime
      });
    }
    else emit sendError("交易登录失败");
}

void TdApi::fetchAllInstruments()
{
    CThostFtdcQryInstrumentField a = {{0}};
    int code=api->ReqQryInstrument(&a,++nRequestID);
    iDebug<<"正在查询所有合约"<<Util::convertApiReturnValueToText(code);
}
void TdApi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    // 只要长度6位且后四位为如下条件的，这样少一点
    if (strlen(pInstrument->InstrumentID)==6&&strcmp(&(pInstrument->InstrumentID[2]),"2203")==0) {
        instruments.push_back(InstrumentField{
            pInstrument->InstrumentID,
            QString::fromLocal8Bit(pInstrument->InstrumentName)
        });
    }
    if (bIsLast) {
        if (pRspInfo==nullptr) iDebug<<"查询所有合约响应"<<"未知";
        else rDebug("查询所有合约响应",pRspInfo->ErrorMsg);
        emit sendAllInstruments(instruments);
    }
}

void TdApi::reqQryTradingAccount()
{
    CThostFtdcQryTradingAccountField a = {{0}};
    strcpy_s(a.BrokerID, userInfo.brokerId.toStdString().c_str());
    strcpy_s(a.InvestorID, userInfo.userId.toStdString().c_str());
    strcpy_s(a.CurrencyID, "CNY");
    int code=api->ReqQryTradingAccount(&a,++nRequestID);
    iDebug<<"请求查询资金账户"<<Util::convertApiReturnValueToText(code);
}

void TdApi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pRspInfo==nullptr) iDebug<<"查询资金账户响应"<<"未知";
    else rDebug("查询资金账户响应",pRspInfo->ErrorMsg); // pRspInfo为nullptr，很奇怪
    emit sendTradingAccount(TradingAccount{
        pTradingAccount->Available,
        pTradingAccount->FrozenCash,
        pTradingAccount->WithdrawQuota
    });
}
void TdApi::reqQryInvestorPosition()
{
    CThostFtdcQryInvestorPositionField a = {{0}};
    strcpy_s(a.BrokerID, userInfo.brokerId.toStdString().c_str());
    strcpy_s(a.InvestorID, userInfo.userId.toStdString().c_str());
    int code=api->ReqQryInvestorPosition(&a, ++nRequestID);
    iDebug<<"请求查询投资者持仓"<<Util::convertApiReturnValueToText(code);
}

void TdApi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pInvestorPosition!=nullptr) {
        emit sendInvestorPosition(InvestorPosition{
            pInvestorPosition->InstrumentID,
            pInvestorPosition->Position,
            pInvestorPosition->PositionProfit
        });
    }
    if (bIsLast) {
        if (pRspInfo==nullptr) iDebug<<"查询投资者持仓响应"<<"未知";
        else rDebug("查询投资者持仓响应",pRspInfo->ErrorMsg);
    }
}

void TdApi::reqOrderInsert()
{

}

void TdApi::test1(){
    emit sendInvestorPosition(InvestorPosition{
      "test"+QString::number(qrand()),
      1234,
      1234.56
  });
}
void TdApi::test2(){
    emit sendOrder(Order{
      "test"+QString::number(qrand()),
      "Test",
      char((int)'0'+qrand()%2),
      1234.56,
      1234,
      5678,
       '0',
       "12:00:01"
  });
}
