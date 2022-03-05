#include "util.h"

Util::Util()
{

}

QString Util::convertApiReturnValueToText(int val)
{
    switch (val) {
    case 0:
        return "发送请求成功";
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

QString Util::convertDirectionToText(char dir)
{
    switch (dir) {
    case '0':
        return "买";
    case '1':
        return "卖";
    default:
        return "未知";
    }
}

QString Util::convertOrderStatusToText(char status)
{
    switch (status) {
    case '0':
        return "全部成交";
    case '5':
        return "撤单";
    default:
        return "未知";
    }
}

