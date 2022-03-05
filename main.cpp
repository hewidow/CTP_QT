#include "mainwindow.h"

#include <QApplication>
#include "debug.h"
#include "util.h"

MainWindow *w;

void logOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    // w->logOutput(QString().sprintf("[%s][%s:%d][%s]",qPrintable(QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss")),context.file,context.line,Util::convertQtMsgTypeToText(type).toStdString().data())+msg);
    w->logOutput(QString().sprintf("[%s][%s]",qPrintable(QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss")),Util::convertQtMsgTypeToText(type).toStdString().data())+msg);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString qss;
    QFile qssFile(":/main.qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        qss = QLatin1String(qssFile.readAll());
        qApp->setStyleSheet(qss);
        qssFile.close();
    } else {
        iDebug<<"未找到全局样式";
    } // 设置全局样式

    // qInstallMessageHandler(logOutput); // 注册日志输出捕获

    w=new MainWindow();
    int code=a.exec();
    delete w;
    return code;
}
