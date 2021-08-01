#ifndef PLUGINS_H
#define PLUGINS_H

#include <dlfcn.h>
#include <unistd.h>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QVector>

#include "BasePlugin.hh"
#include "errnos.h"

class Plugin {
public:
    QString _name;
    QString _keyword;
    QString _version;
    bool cmd;
    bool _enable;

    QString path;
    void* dlfd;
    BasePlugin* plugin;
    int errCode;

    Plugin(QString name, QString keyword, QString version, bool cmd, bool enable);
    ~Plugin();

private:
    create_t* dlPluginCreate;
    destroy_t* dlPluginDestroy;

    void checkSo();
    void opendl();
};

#endif  // PLUGINS_H
