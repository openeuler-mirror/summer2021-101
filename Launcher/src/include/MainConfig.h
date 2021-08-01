#ifndef MAINCONFIG_H
#define MAINCONFIG_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QVector>
#include <unistd.h>

#include "BasePlugin.hh"
#include "Plugin.h"
#include "errnos.h"

// 读取整个界面程序的配置。根据配置生成全部插件类
class MainConfig {
public:
    MainConfig(QString file);
    ~MainConfig();

    QVector<Plugin*>& getPlugins();

private:
    QFile* fd;
    int status;
    QVector<Plugin*> plugins;
    QString version;

    void checkConf();
    void openPlugins();
};

#endif // MAINCONFIG_H
