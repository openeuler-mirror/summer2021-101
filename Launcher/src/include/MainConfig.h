#ifndef MAINCONFIG_H
#define MAINCONFIG_H

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
#include "Plugin.h"
#include "errnos.h"

// 读取整个界面程序的配置。根据配置生成全部插件类
class MainConfig : public QObject {
    Q_OBJECT
public:
    MainConfig(QString file, QVector<Plugin*>& value);
    ~MainConfig();

    QVector<Plugin*>& getPlugins();

    QString getVersion() const;

public slots:
    void reWrite();

private:
    QFile* fd;
    int status;
    QVector<Plugin*>& plugins;
    QString version;

    void checkConf();
    void openPlugins();
};

#endif  // MAINCONFIG_H
