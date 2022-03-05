#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>
#include <QDateTime>

// 默认debug输出
#define iDebug qDebug().noquote()
// 输出api接口的错误信息
#define rDebug(k,e) iDebug<<k<<QString::fromLocal8Bit(e->ErrorMsg)

class Debug
{
public:
    Debug();
};
#endif // DEBUG_H
