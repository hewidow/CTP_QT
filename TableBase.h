#ifndef TableBase_H
#define TableBase_H

#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include "util.h"
#include "debug.h"

class TableBase : public QTableView
{
    Q_OBJECT
public:
    explicit TableBase(QWidget *parent = nullptr,QList<QString> _headers={});
    void clearData(); // 清空数据
protected:
    QStandardItemModel* model;
    QList<QString>headers;
};

#endif // TableBase_H
