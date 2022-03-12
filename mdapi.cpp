#include "mdapi.h"

MdApi::MdApi(QObject *parent) : QObject(parent)
{
    api=nullptr;
}
void MdApi::connect(LoginField u)
{
    QDir dir;
    if (!dir.exists(SUBSCRIBE_INFO_PATH)) dir.mkpath(SUBSCRIBE_INFO_PATH);

    api=CThostFtdcMdApi::CreateFtdcMdApi(SUBSCRIBE_INFO_PATH.toStdString().c_str()); // 初始化api
    api->RegisterSpi(this); // 绑定回调接口
    api->RegisterFront(Util::convertQStringToCharPoint(u.mdAddress)); // 设置连接地址
    api->Init(); // 初始化运行环境
    iDebug<<"行情地址"<<u.mdAddress;
    iDebug<<"设置行情API → 成功";
}
void MdApi::release()
{
	iDebug << "释放行情API → 成功";
    if (api!=nullptr) {
        api->RegisterSpi(NULL);
        api->Release();
        api=nullptr;
    }
}
void MdApi::login()
{
    CThostFtdcReqUserLoginField t={ 0 };
    api->ReqUserLogin(&t, ++nRequestID);
}
void MdApi::subscribe(QVector<InstrumentField>instruments)
{
    int len=instruments.size();
    std::sort(instruments.begin(),instruments.end(),[](const InstrumentField &x,const InstrumentField &y){
        return x.InstrumentID<y.InstrumentID;
    }); // 按合约id排序
    char **pInstrument=new char *[len];
    for (int i=0;i<len;++i) {
        instrumentsMap[instruments[i].InstrumentID]=instruments[i];
        pInstrument[i]=Util::convertQStringToCharPoint(instruments[i].InstrumentID);
    }
    int val=api->SubscribeMarketData(pInstrument, len);
    iDebug<<"订阅合约总数"<<len;
    iDebug<<"请求订阅合约"<<Util::convertApiReturnValueToText(val);
}

void MdApi::OnFrontConnected()
{
    iDebug<<"行情连接成功";
    emit sendConnectionStatus(true);
    login();
}
void MdApi::OnFrontDisconnected(int nReason)
{
    iDebug<<"行情连接断开"<<"错误码:"<<QString::number(nReason,16);
    emit sendConnectionStatus(false);
}
void MdApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    rDebug("行情登录响应",pRspInfo);
    if (pRspInfo->ErrorID==0) emit sendRspLogin(CThostFtdcRspUserLoginField{});
    else emit sendError("行情登录失败");
}
void MdApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    // iDebug<<"订阅合约响应"<<pDepthMarketData->InstrumentID<<pDepthMarketData->LastPrice;
    InstrumentField tp=instrumentsMap[pDepthMarketData->InstrumentID];
    emit sendRtnDepthMarketData(QuoteField{
        tp.ExchangeID,
        pDepthMarketData->InstrumentID,
        tp.InstrumentName,
        pDepthMarketData->UpdateTime,
        pDepthMarketData->LastPrice,
        DBL_MAX==pDepthMarketData->PreClosePrice?DBL_MAX:pDepthMarketData->LastPrice-pDepthMarketData->PreClosePrice,
        DBL_MAX==pDepthMarketData->PreClosePrice?DBL_MAX:(pDepthMarketData->LastPrice-pDepthMarketData->PreClosePrice)/pDepthMarketData->PreClosePrice*100,
        pDepthMarketData->BidPrice1,
        pDepthMarketData->BidVolume1,
        pDepthMarketData->AskPrice1,
        pDepthMarketData->AskVolume1,
        pDepthMarketData->Volume,
        pDepthMarketData->OpenPrice,
        pDepthMarketData->HighestPrice,
        pDepthMarketData->LowestPrice
    });
}

