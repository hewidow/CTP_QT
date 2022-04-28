#include "mainwindow.h"

#include <QApplication>
#include <QDateTime>
#include "debug.h"
#include "util.h"
#include "config.h"

MainWindow* w;

void logOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
	// w->logOutput(QString().sprintf("[%s][%s:%d][%s]",qPrintable(QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss")),context.file,context.line,Util::convertQtMsgTypeToText(type).toStdString().data())+msg);
	// w->logOutput(QString().sprintf("[%s][%s]", qPrintable(QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss")), Util::convertQtMsgTypeToText(type).toStdString().data()) + msg);
	// 以上为更详细的调试信息
	w->logOutput(QString().sprintf("[%s]", qPrintable(QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss"))) + msg);
}
// 初始化配置
void initConfig() {
	QFile file;
	if (!file.exists(CONFIG_PATH)) {
		configInstance->set("INSTRUMENT_ID_LEN", 6);
		configInstance->set("INSTRUMENT_ID_FILTER", "205");
	}
}
int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	w = new MainWindow();

	// 重定向日志输出
	qInstallMessageHandler(logOutput);

	// 设置全局样式
	QString qss;
	QFile qssFile(":/qss/main.qss");
	qssFile.open(QFile::ReadOnly);
	if (qssFile.isOpen())
	{
		qss = QLatin1String(qssFile.readAll());
		qApp->setStyleSheet(qss);
		qssFile.close();
	}
	else {
		iDebug << "未找到全局样式";
	}

	initConfig();

	int code = a.exec();
	delete w;
	return code;
}
