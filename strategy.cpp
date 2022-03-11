#include "strategy.h"

Strategy::Strategy(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

    QDir dir;
    if (!dir.exists(STRATEGY_PATH)) dir.mkpath(STRATEGY_PATH);

    dir.setPath(STRATEGY_PATH);

    dir.setFilter(QDir::Files);
    QStringList filters;
    filters << "*.cpp";
    dir.setNameFilters(filters);
    QStringList names = dir.entryList();

    model= new QStandardItemModel(this);;
    for (auto &name : names) {
        model->appendRow(new QStandardItem(name));
    }
    ui.listView->setModel(model);
    ui.listView->setEditTriggers(QAbstractItemView::NoEditTriggers); // ½ûÖ¹±à¼­
    connect(ui.listView, &QListView::doubleClicked, this, &Strategy::doubleClicked); // °ó¶¨Ë«»÷ÊÂ¼þ
}

Strategy::~Strategy()
{

}

void Strategy::doubleClicked(QModelIndex index)
{
    QString path = STRATEGY_PATH + index.data().toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(path).absoluteFilePath()));
}
