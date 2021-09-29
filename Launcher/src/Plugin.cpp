#include "Plugin.h"

Plugin::Plugin(QString name, QString keyword, QString version, bool cmd, bool enable) {
    this->_name = name;
    this->_keyword = keyword;
    this->_version = version;
    this->_enable = enable;
    this->cmd = cmd;
    this->errCode = 0;
    this->dlfd = NULL;
    this->dlpath = QDir::currentPath() + "/plugins/" + this->_name + "/" + this->_name + "." + this->_version + ".so";
    this->dlPluginCreate = NULL;
    this->dlPluginDestroy = NULL;
    checkSo();

    if (this->_enable) {
        opendl();
    }
}

Plugin::~Plugin() {
    if (this->plugin != NULL) {
        this->dlPluginDestroy(this->plugin);
        this->plugin = NULL;
    }
    if (this->dlfd != NULL) {
        dlclose(this->dlfd);
        this->dlfd = NULL;
    }
}

void Plugin::checkSo() {
    if (this->_enable) {
        if (::access(this->dlpath.toLocal8Bit().data(), R_OK)) {
            this->_enable = false;
            this->errCode = ERR_DL_FOUND;
        }
    }
}

void Plugin::opendl() {
    this->dlfd = dlopen(this->dlpath.toLocal8Bit().data(), RTLD_LAZY);
    if (!dlfd) {
        this->_enable = false;
        this->errCode = ERR_DL_OPEN;
        qDebug() << "[Error]: DL Open Error";
        return;
    }

    this->dlPluginCreate = (create_t*)dlsym(this->dlfd, "create");
    if (!dlPluginCreate) {
        this->_enable = false;
        this->errCode = ERR_DL_PLUGIN;
        qDebug() << "[Error]: DL Create find Error";

        return;
    }

    this->dlPluginDestroy = (destroy_t*)dlsym(this->dlfd, "destroy");
    if (!dlPluginDestroy) {
        this->_enable = false;
        this->errCode = ERR_DL_CLOSE;
        qDebug() << "[Error]: DL close find Error";
        return;
    }
    this->plugin = this->dlPluginCreate();

    qDebug() << "[Info]: Load Plugin " << this->_name << "OK";
}
