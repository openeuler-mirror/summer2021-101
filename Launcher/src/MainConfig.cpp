#include "include/MainConfig.h"

MainConfig::MainConfig(QString file, QVector<Plugin *> &value) : plugins(value) {
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

// QVector<Plugin *> &MainConfig::getPlugins() { return this->plugins; }

QString MainConfig::getVersion() const { return version; }

void MainConfig::checkConf() {
    this->plugins.clear();
    if (!this->fd->open(QIODevice::ReadWrite)) {
        qDebug() << "[Error]: Conf Read Error" << this->fd->errorString();
        status = ERR_CONF;
        return;
    }

    QByteArray rawData = this->fd->readAll();
    QJsonParseError jsErr;
    QJsonObject obj;
    QJsonDocument document = QJsonDocument::fromJson(rawData, &jsErr);
    QJsonArray conf;

    if (document.isNull() || jsErr.error != QJsonParseError::NoError || !document.isObject()) {
        qDebug() << "[Error]: Conf Syntax Error" << jsErr.errorString();
        status = ERR_CONF;
    } else {
        obj = document.object();
        if (!obj.contains("Version") || !obj.contains("Plugins")) {
            qDebug() << "[CONF] ERROR:"
                     << "json error";
            status = ERR_CONF;
        } else {
            if (obj.value("Version").isString()) {
                this->version = obj.value("Version").toString();
            } else {
                this->status = ERR_CONF;
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
                            continue;

                        } else {
                            if (!onePlu.value("name").isString() || !onePlu.value("keyword").isString() ||
                                !onePlu.value("version").isString() || !onePlu.value("cmd").isBool() ||
                                !onePlu.value("status").isBool()) {
                                continue;

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
    qDebug() << "[Info]: Check Config status:" << this->status;
}

void MainConfig::reWrite() {
    QJsonObject obj;
    obj.insert("Version", this->version);
    QJsonArray array;
    qDebug() << this->plugins.count();
    for (int i = 0; i < this->plugins.count(); ++i) {
        QJsonObject onePlugin;

        onePlugin.insert("name", this->plugins.at(i)->_name);
        onePlugin.insert("version", this->plugins.at(i)->_version);

        onePlugin.insert("keyword", this->plugins.at(i)->_keyword);
        onePlugin.insert("cmd", this->plugins.at(i)->cmd);
        onePlugin.insert("status", this->plugins.at(i)->_enable);
        array.append(onePlugin);
    }

    obj.insert("Plugins", array);
    QJsonDocument dom(obj);

    this->fd->resize(0);
    this->fd->write(dom.toJson());
    this->fd->flush();

    qDebug() << "[Info]: Config Rewrite OK";
}

int MainConfig::getStatus() const { return status; }
