#ifndef UTIL_H
#define UTIL_H

#include <QString>


class Util
{
public:
    Util();
    static char *convertQStringToCharPoint(QString,int len=256);
    static QString convertApiReturnValueToText(int);
    static QString convertNumberToQString(double,int prec=-1);
    static QString convertNumberToQString(int);
    static QString convertQtMsgTypeToQString(QtMsgType);
};

#endif // UTIL_H
