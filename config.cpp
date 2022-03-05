#include "config.h"

Config *Config::config=nullptr;
QSettings *Config::settings=nullptr;

Config* Config::getInstance()
{
    if (config==nullptr)
    {
        config=new Config();
        settings=new QSettings(CONFIG_PATH,QSettings::IniFormat);
    }
    return config;
}
QVariant Config::get(QString key)
{
    return settings->value(key);
}
void Config::set(QString key,QVariant value)
{
    settings->setValue(key,value);
}
Config::Config()
{

}
