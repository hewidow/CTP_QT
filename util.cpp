#include "util.h"

Util::Util()
{

}

QString Util::convertApiReturnValueToText(int val)
{
    switch (val) {
    case 0:
        return "成功";
    case -1:
        return "网络连接失败";
    case -2:
        return "未处理请求超过许可数";
    case -3:
        return "每秒发送请求数超过许可数";
    default:
        return "未知";
    }
}


char *Util::convertQStringToCharPoint(QString str,int len)
{
    char *dst=new char[len];
    strcpy_s(dst,len,str.toStdString().c_str());
    return dst;
}

QString Util::convertNumberToQString(int v)
{
    return QString::number(v);
}

QString Util::convertNumberToQString(double v,int prec)
{
    if (v==DBL_MAX) return "-";
    if (prec!=-1) return QString::number(v,'f',prec);
    return QString::number(v);
}

QString Util::convertQtMsgTypeToText(QtMsgType type)
{
    switch (type) {
    case QtDebugMsg:
        return "Debug";
    case QtInfoMsg:
        return "Info";
    case QtWarningMsg:
        return "Warning";
    case QtCriticalMsg:
        return "Critical";
    case QtFatalMsg:
        return "Fatal";
    default:
        return "Unknown";
    }
}

QString Util::convertDirectionToText(TThostFtdcDirectionType dir)
{
    switch (dir) {
    case THOST_FTDC_D_Buy:
        return "买";
    case THOST_FTDC_D_Sell:
        return "卖";
    default:
        return "未知";
    }
}

QString Util::convertOrderStatusToText(TThostFtdcOrderStatusType status)
{
    switch (status) {
    case THOST_FTDC_OST_AllTraded:
        return "全部成交";
    case THOST_FTDC_OST_PartTradedQueueing:
        return "部分成交还在队列中";
    case THOST_FTDC_OST_PartTradedNotQueueing:
        return "部分成交不在队列中";
    case THOST_FTDC_OST_NoTradeQueueing:
        return "未成交还在队列中";
    case THOST_FTDC_OST_NoTradeNotQueueing:
        return "未成交不在队列中";
    case THOST_FTDC_OST_Canceled:
        return "撤单";
    case THOST_FTDC_OST_Unknown:
        return "未知";
    case THOST_FTDC_OST_NotTouched:
        return "尚未触发";
    case THOST_FTDC_OST_Touched:
        return "已触发";
    default:
        return "未定义";
    }
}

