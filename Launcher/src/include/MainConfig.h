#ifndef MAINCONFIG_H
#define MAINCONFIG_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QFile>
#include <QDebug>
#include <QVector>
#include <unistd.h>
#include <QDir>

#include "errnos.h"
#include "BasePlugin.hh"
#include "Plugin.h"



class MainConfig
{
public:
    MainConfig(QString file);
    ~MainConfig();

    QVector<Plugin *> &getPlugins();
private:
    QFile *fd;
    int status;
    QVector<Plugin *> plugins;
    QString version;

    void checkConf();
    void openPlugins();



};

#endif // MAINCONFIG_H
