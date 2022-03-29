#include "TableBase.h"

TableBase::TableBase(QWidget* parent, QList<QString> _headers) : QTableView(parent)
{
	model = new QStandardItemModel(parent);
	// 设置表头
	headers.clear();
	headers.append(_headers);
	model->setColumnCount(headers.size());
	for (int i = 0; i < headers.size(); ++i) model->setHeaderData(i, Qt::Horizontal, headers[i]);
	// 设置model
	setModel(model);

	horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 表头均分填满表格
	horizontalHeader()->setHighlightSections(false); // 选中时表头不会变粗
	verticalHeader()->setVisible(false); // 隐藏列头
	setSelectionBehavior(QAbstractItemView::SelectRows); //设置选中模式为选中行
	setFocusPolicy(Qt::NoFocus); // 去除选中虚线框
	setShowGrid(false); // 去除格子线
	setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
}

void TableBase::clearData()
{
	model->removeRows(0, model->rowCount());
}
