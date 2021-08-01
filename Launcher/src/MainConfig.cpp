#include "include/MainConfig.h"

MainConfig::MainConfig(QString file) {
    QDir::setCurrent("/home/miao/Documents/github/summer2021-101/Launcher/");

    this->status = 0;
    this->fd = new QFile(file);
    checkConf();
}

MainConfig::~MainConfig() {
    if (this->fd != NULL) {
        this->fd->close();
        this->fd = NULL;
    }
}

QVector<Plugin *> &MainConfig::getPlugins() { return this->plugins; }

void MainConfig::checkConf() {
    this->plugins.clear();
    this->fd->open(QIODevice::ReadWrite);

    QByteArray rawData = this->fd->readAll();
    QJsonParseError jsErr;
    QJsonObject obj;
    QJsonDocument document = QJsonDocument::fromJson(rawData, &jsErr);
    QJsonArray conf;

    if (document.isNull() || jsErr.error != QJsonParseError::NoError || !document.isObject()) {
        qDebug() << "[CONF] ERROR:" << jsErr.error << jsErr.errorString();
        status = ERR_CONF;
    } else {
        obj = document.object();
        if (!obj.contains("Version") || !obj.contains("Plugins")) {
            qDebug() << "[CONF] ERROR:"
                     << "json error";
            status = ERR_CONF;
        } else {
            if (obj.value("version").isString()) {
                this->version = obj.value("version").toString();
            }
            if (!obj.value("Plugins").isArray()) {
                this->status = ERR_CONF;
            } else {
                conf = obj.value("Plugins").toArray();
                for (int i = 0; i < conf.count(); ++i) {
                    if (!conf.at(i).isObject()) {
                    } else {
                        QJsonObject onePlu = conf.at(i).toObject();
                        if (!onePlu.contains("name") || !onePlu.contains("keyword") || !onePlu.contains("version") ||
                            !onePlu.contains("cmd") || !onePlu.contains("status")) {
                            this->status = ERR_CONF;

                        } else {
                            if (!onePlu.value("name").isString() || !onePlu.value("keyword").isString() ||
                                !onePlu.value("version").isString() || !onePlu.value("cmd").isBool() ||
                                !onePlu.value("status").isBool()) {
                                this->status = ERR_CONF;
                            } else {
                                Plugin *onePlugin =
                                    new Plugin(onePlu.value("name").toString(), onePlu.value("keyword").toString(),
                                               onePlu.value("version").toString(), onePlu.value("cmd").toBool(),
                                               onePlu.value("status").toBool());
                                this->plugins.append(onePlugin);
                            }
                        }
                    }
                }
            }
        }
    }
    qDebug() << "conf read status:" << this->status;
}
