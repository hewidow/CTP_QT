#include "tdapi.h"

TdApi::TdApi(QObject *parent) : QObject(parent)
{
   api=nullptr;
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

void TdApi::release()
{
    if (api!=nullptr) {
        iDebug<<"释放交易连接";
        api->Release();
    }
}

void TdApi::reqAuthenticate()
{
    CThostFtdcReqAuthenticateField a = {{0}};
    strcpy_s(a.BrokerID, userInfo.brokerId.toStdString().c_str());
    strcpy_s(a.UserID, userInfo.account.toStdString().c_str());
    strcpy_s(a.AuthCode, userInfo.authCode.toStdString().c_str());
    strcpy_s(a.AppID, userInfo.appId.toStdString().c_str());
    api->ReqAuthenticate(&a, nRequestID++);
}

void TdApi::login()
{
    CThostFtdcReqUserLoginField t={{0}};
    strcpy_s(t.BrokerID,userInfo.brokerId.toStdString().c_str());
    strcpy_s(t.UserID,userInfo.account.toStdString().c_str());
    strcpy_s(t.Password,userInfo.password.toStdString().c_str());
    api->ReqUserLogin(&t, nRequestID++);
}

void TdApi::fetchAllInstruments()
{
    iDebug<<"正在查询所有合约";
    CThostFtdcQryInstrumentField a = {{0}};
    api->ReqQryInstrument(&a, nRequestID++);
}

void TdApi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
//    rDebug("查询所有合约响应",pRspInfo); // pRspInfo为空指针
    // 只要长度6位且后四位为如下条件的，这样少一点
    if (strlen(pInstrument->InstrumentID)==6&&strcmp(&(pInstrument->InstrumentID[2]),"2203")==0) {
        instruments.push_back(InstrumentField{
            pInstrument->InstrumentID,
            QString::fromLocal8Bit(pInstrument->InstrumentName)
        });
    }
    if (bIsLast) emit sendAllInstruments(instruments);
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
void TdApi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    rDebug("认证响应",pRspInfo);
    if (pRspInfo->ErrorID==0) login();
    else emit sendError("交易认证失败");
}
void TdApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    rDebug("交易登录响应",pRspInfo);
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
