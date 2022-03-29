#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>

// 默认debug输出
#define iDebug qDebug().noquote()
// 输出api接口的错误信息
#define rDebug(k,e) iDebug<<k<<"→"<<(e==nullptr?"成功":QString::fromLocal8Bit(e->ErrorMsg))

class Debug
{
public:
	Debug();
};
#endif // DEBUG_H
