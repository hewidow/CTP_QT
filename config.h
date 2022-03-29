#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QSettings>
#include "global.h"

#define configInstance Config::getInstance()

class Config
{
public:
	static Config* getInstance(); // 单例模式
	static QVariant get(QString);
	static void set(QString, QVariant);
private:
	Config(); // 私有构造函数
	Config(const Config&); // 私有拷贝构造函数
	const Config& operator=(const Config&); // 私有赋值拷贝构造函数
private:
	static Config* config;
	static QSettings* settings;
};

#endif // CONFIG_H
