#include "StrategyBase.h"

void StrategyBase::log(QString s)
{
    iDebug << "【当前策略："+name()+"】" << s;
}
void StrategyBase::buy(QString InstrumentID, double LastPrice, int VolumeTotalOriginal)
{
    CThostFtdcInputOrderField ord = { 0 };
    strcpy_s(ord.InstrumentID, InstrumentID.toStdString().c_str());
    ord.LimitPrice = LastPrice;
    ord.VolumeTotalOriginal = VolumeTotalOriginal;
    ord.Direction = THOST_FTDC_D_Buy;
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice; // 限价单
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open; // 开仓
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation; // 投机
    ord.TimeCondition = THOST_FTDC_TC_GFD; // 当日有效
    ord.VolumeCondition = THOST_FTDC_VC_AV; //任意数量
    ord.MinVolume = 1; // 最小成交量
    ord.ContingentCondition = THOST_FTDC_CC_Immediately; // 触发条件
    ord.StopPrice = 0; // 止损价
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose; // 强平原因
    ord.IsAutoSuspend = 0; // 自动挂起标志
    sendReqOrderInsert(ord);
    log("买入开仓 | 代码 " + QString(ord.InstrumentID) + " | 价格 " + QString::number(ord.LimitPrice) + " | 数量 " + QString::number(ord.VolumeTotalOriginal));
}
void StrategyBase::sell(QString InstrumentID, double LastPrice, int VolumeTotalOriginal)
{
    CThostFtdcInputOrderField ord = { 0 };
    strcpy_s(ord.InstrumentID, InstrumentID.toStdString().c_str());
    ord.LimitPrice = LastPrice;
    ord.VolumeTotalOriginal = VolumeTotalOriginal;
    ord.Direction = THOST_FTDC_D_Buy;
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice; // 限价单
    ord.CombOffsetFlag[0] = THOST_FTDC_OF_Close; // 平仓
    ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation; // 投机
    ord.TimeCondition = THOST_FTDC_TC_GFD; // 当日有效
    ord.VolumeCondition = THOST_FTDC_VC_AV; //任意数量
    ord.MinVolume = 1; // 最小成交量
    ord.ContingentCondition = THOST_FTDC_CC_Immediately; // 触发条件
    ord.StopPrice = 0; // 止损价
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose; // 强平原因
    ord.IsAutoSuspend = 0; // 自动挂起标志
    emit sendReqOrderInsert(ord);
    log("卖出平仓 | 代码 " + QString(ord.InstrumentID) + " | 价格 " + QString::number(ord.LimitPrice) + " | 数量 " + QString::number(ord.VolumeTotalOriginal));
}
void StrategyBase::cancel(TThostFtdcOrderSysIDType OrderSysID)
{
    log("取消报单 | 报单编号 "+ QString(OrderSysID));
    CThostFtdcInputOrderActionField a = { 0 };
    strcpy_s(a.OrderSysID, OrderSysID);
    a.ActionFlag = THOST_FTDC_AF_Delete;

    emit sendReqOrderAction(a);
}