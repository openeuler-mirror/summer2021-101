#include "Plugin.h"

Plugin::Plugin(QString name, QString keyword, QString version, bool cmd, bool enable) {
    this->_name = name;
    this->_keyword = keyword;
    this->_version = version;
    this->_enable = enable;
    this->errCode = 0;
    this->dlfd = NULL;
    this->path = QDir::currentPath() + "/plugins/" + this->_name + "/" + this->_name + "." + this->_version + ".so";
    checkSo();
    if (this->_enable) {
        opendl();
    }
}

Plugin::~Plugin() {
    if (this->dlfd != NULL) {
        dlclose(this->dlfd);
        this->dlfd = NULL;
    }
    if (this->plugin != NULL) {
        this->dlPluginDestroy(this->plugin);
        this->plugin = NULL;
    }
}

void Plugin::checkSo() {
    if (this->_enable) {
        //        qDebug()<<"one plugin so path:"<<this->path;
        if (::access(this->path.toLocal8Bit().data(), R_OK)) {
            this->_enable = false;
            this->errCode = ERR_DL_FOUND;
            qDebug() << "check so status:" << this->_enable;
        }
    }
}

void Plugin::opendl() {
    this->dlfd = dlopen(this->path.toLocal8Bit().data(), RTLD_LAZY);
    if (!dlfd) {
        this->_enable = false;
        this->errCode = ERR_DL_OPEN;
        qDebug() << dlerror();
        return;
    }
    this->dlPluginCreate = (create_t*)dlsym(this->dlfd, "create");
    if (!dlPluginCreate) {
        this->_enable = false;
        this->errCode = ERR_DL_PLUGIN;
        qDebug() << dlerror();

        return;
    }

    this->dlPluginDestroy = (destroy_t*)dlsym(this->dlfd, "destroy");
    if (!dlPluginDestroy) {
        this->_enable = false;
        this->errCode = ERR_DL_CLOSE;
        qDebug() << dlerror();
        return;
    }

    this->plugin = this->dlPluginCreate();
    qDebug() << "open one plugin successfully";
    qDebug() << QString::fromStdString(this->plugin->getinfo());
}
