#ifndef QUOTETABLE_H
#define QUOTETABLE_H

#include "basetable.h"
#include "api/ThostFtdcUserApiStruct.h"
#include "debug.h"

class QuoteTable : public BaseTable
{
public:
    explicit QuoteTable(QWidget *parent = nullptr);
    QList<QString>formatData(int,QuoteField);
    void updateQuote(QuoteField); // 更新行情
    int findRowIndex(QString); // 返回合约id对应的行数
private:
    QMap<QString,int>instrumentIdToRowIndex;
};

#endif // QUOTETABLE_H
