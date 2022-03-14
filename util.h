#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include "global.h"

class Util
{
public:
    Util();
    static char *convertQStringToCharPoint(QString,int len=256);
    static QString convertApiReturnValueToText(int);
    static QString convertNumberToQString(double,int prec=-1);
    static QString convertNumberToQString(int);
    static QString convertQtMsgTypeToText(QtMsgType);
    static QString convertDirectionToText(TThostFtdcDirectionType);
    static QString convertOrderStatusToText(TThostFtdcOrderStatusType);
    static QString convertPosiDirectionToText(TThostFtdcPosiDirectionType);
    static QString convertCombOffsetFlagToText(TThostFtdcCombOffsetFlagType);
};

#endif // UTIL_H
