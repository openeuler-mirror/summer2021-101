#include "include/PluginManager.h"

#include "ui_PluginManager.h"

PluginManager::PluginManager(QVector<Plugin *> &value, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::PluginManager), plugins(value) {
    ui->setupUi(this);

    this->contentWidget = new QWidget();
    this->layout = new QVBoxLayout();
    ui->scrollArea->setWidget(this->contentWidget);
    this->layout->setAlignment(Qt::AlignTop);
    this->contentWidget->resize(1024, 1024);
    this->contentWidget->setLayout(this->layout);

    setPluginManagePanels();
    QObject::connect(ui->newPlugin, &QPushButton::clicked, this, &PluginManager::addNewPlugin);
    QObject::connect(ui->refresh, &QPushButton::clicked, this, &PluginManager::refresh);
}

PluginManager::~PluginManager() { delete ui; }

void PluginManager::closeEvent(QCloseEvent *event) {
    this->hide();

    event->ignore();
}

void PluginManager::setPluginManagePanels() {
    for (int i = 0; i < this->plugins.count(); ++i) {
        PluginManagePanel *onePlu = new PluginManagePanel(this->plugins.at(i));
        this->contents.push_back(onePlu);
    }

    for (int i = 0; i < this->contents.count(); ++i) {
        this->layout->addWidget(contents.at(i));
    }
}

void PluginManager::addNewPlugin() {
    QString conf = QFileDialog::getOpenFileName(
        this, "select one plugin", "/home/miao/Documents/github/summer2021-101/plugins_def/test/", "*.json", 0, 0);

    qDebug() << conf;
    QFile fd(conf);
    fd.open(QIODevice::ReadOnly);

    QJsonDocument dom;
    QJsonParseError jsErr;
    dom = QJsonDocument::fromJson(fd.readAll(), &jsErr);
    if (dom.isNull() || jsErr.error != QJsonParseError::NoError || !dom.isObject()) {
        qDebug() << "Error parse ";
        QMessageBox::information(this, "Error", "Error in parse plugin", QMessageBox::Yes, QMessageBox::Yes);
    } else {
        QJsonObject obj = dom.object();
        if (!obj.contains("name") || !obj.contains("elf.so") || !obj.contains("keyword") || !obj.contains("version") ||
            !obj.contains("cmd")) {
            qDebug() << "Error parse ";
            QMessageBox::information(this, "Error", "Error in parse plugin", QMessageBox::Yes, QMessageBox::Yes);
        } else if (!obj.value("name").isString() || !obj.value("keyword").isString() ||
                   !obj.value("version").isString() || !obj.value("cmd").isBool() || !obj.value("elf.so").isString()) {
            qDebug() << "Error parse ";
            QMessageBox::information(this, "Error", "Error in parse plugin", QMessageBox::Yes, QMessageBox::Yes);
        } else {
            int index = isUpdate(obj.value("name").toString());
            if (index != -1) {
                for (int i = 0; i < this->layout->count(); ++i) {
                    this->layout->removeItem(this->layout->itemAt(0));
                }

                for (int i = 0; i < this->contents.length(); ++i) {
                    this->contents.at(i)->setParent(nullptr);
                    this->contents.at(i)->deleteLater();
                }
                this->contents.clear();

                qDebug() << "index" << index;
                Plugin *oldplu = this->plugins.at(index);
                this->plugins.remove(index);
                oldplu->~Plugin();

            } else {
            }

            if (cp_r(conf.section("/", 0, -2), QDir::currentPath() + "/plugins/" + obj.value("name").toString())) {
                qDebug() << "Plugin add OK";
            } else {
                qDebug() << "error when add new plugin";
            }
            Plugin *newPlugin = new Plugin(obj.value("name").toString(), obj.value("keyword").toString(),
                                           obj.value("version").toString(), obj.value("cmd").toBool(), true);
            qDebug() << newPlugin->errCode;
            this->plugins.push_back(newPlugin);
        }
    }
    fd.close();
    emit changed();
    refresh();
}

bool PluginManager::cp_r(QString src, QString dst) {
    qDebug() << "cp -r " << src << dst;
    QDir srcDir(src);
    QDir dstDir(dst);

    if (dstDir.exists()) {
        dstDir.removeRecursively();
    }
    if (!dstDir.exists()) {
        dstDir.mkdir(dstDir.absolutePath());
    }

    QFileInfoList list = srcDir.entryInfoList();
    bool ret = true;

    foreach (QFileInfo info, list) {
        if (info.fileName() == "." || info.fileName() == "..") {
            continue;
        }
        if (info.isDir()) {
            // 创建文件夹，递归调用该函数
            cp_r(info.filePath(), dst + "/" + info.fileName());
            continue;
        }
        // 文件拷贝
        QFile file(info.filePath());
        ret = ret && file.copy(dst + "/" + info.fileName());
    }
    qDebug() << ret;
    return ret;
}

void PluginManager::refresh() {
    for (int i = 0; i < this->layout->count(); ++i) {
        this->layout->removeItem(this->layout->itemAt(0));
    }

    for (int i = 0; i < this->contents.length(); ++i) {
        this->contents.at(i)->setParent(nullptr);
        this->contents.at(i)->deleteLater();
    }
    this->contents.clear();
    QTimer::singleShot(200, this, [&]() {
        qDebug() << "refresh";
        setPluginManagePanels();
    });
}

int PluginManager::isUpdate(QString name) {
    for (int i = 0; i < this->plugins.count(); ++i) {
        if (plugins.at(i)->_name.compare(name) == 0) {
            return i;
        }
    }
    return -1;
}
