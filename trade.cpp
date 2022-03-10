﻿#include "trade.h"
#include "ui_trade.h"

Trade::Trade(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Trade)
{
    ui->setupUi(this);
}

Trade::~Trade()
{
    delete ui;
}

void Trade::showDialog(CThostFtdcRspUserLoginField u)
{
    userInfo=u;
    show();
}
void Trade::on_add_clicked()
{
    CThostFtdcInputOrderField ord = { 0 };

    strcpy_s(ord.ExchangeID, ui->ExchangeID->text().toStdString().c_str());
    strcpy_s(ord.InstrumentID, ui->InstrumentID->text().toStdString().c_str());
    ord.Direction = ui->Direction->currentText()=="买"?THOST_FTDC_D_Buy:THOST_FTDC_D_Sell;
    ord.LimitPrice = ui->LimitPrice->text().toDouble();
    ord.VolumeTotalOriginal = ui->VolumeTotalOriginal->text().toInt();

    strcpy_s(ord.BrokerID, userInfo.BrokerID);
    strcpy_s(ord.InvestorID, userInfo.UserID);
    strcpy_s(ord.UserID, userInfo.UserID);
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

    emit sendReqOrderInsert(ord);
}
void Trade::on_del_clicked()
{
    CThostFtdcInputOrderActionField a = { 0 };
    strcpy_s(a.BrokerID, userInfo.BrokerID);
    strcpy_s(a.InvestorID, userInfo.UserID);
    strcpy_s(a.UserID, userInfo.UserID);
    strcpy_s(a.OrderSysID,("        " + ui->OrderSysID->text()).toStdString().c_str());  //对应要撤报单的OrderSysID，有8个空格的前缀
    strcpy_s(a.ExchangeID, ui->ExchangeID->text().toStdString().c_str());
    strcpy_s(a.InstrumentID, ui->InstrumentID->text().toStdString().c_str());
    a.ActionFlag = THOST_FTDC_AF_Delete;

    emit sendReqOrderAction(a);
}