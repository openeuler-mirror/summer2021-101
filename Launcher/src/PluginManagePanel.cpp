#include "include/PluginManagePanel.h"

#include "ui_PluginManagePanel.h"

PluginManagePanel::PluginManagePanel(Plugin* onePlugin, QWidget* parent)
    : QWidget(parent), ui(new Ui::PluginManagePanel) {
    ui->setupUi(this);
    this->onePlugin = onePlugin;
    this->path = QDir::currentPath() + "/plugins/" + onePlugin->_name + "/";
    setupInfo();

    QObject::connect(ui->onoff, &QPushButton::clicked, this, [&]() {
        if (this->onePlugin->_enable) {
            this->onePlugin->_enable = false;
            ui->onoff->setText("OFF");
        }

        else {
            this->onePlugin->_enable = true;
            ui->onoff->setText("ON");
        }
    });
}

PluginManagePanel::~PluginManagePanel() { delete ui; }

void PluginManagePanel::setupInfo() {
    QJsonDocument dom;
    QJsonParseError jsErr;

    if (!this->onePlugin) {
        this->status = ERR_INFO;
        return;
    }
    dom = QJsonDocument::fromJson(this->onePlugin->plugin->getinfo().c_str(), &jsErr);
    if (dom.isNull() || jsErr.error != QJsonParseError::NoError || !dom.isObject()) {
        qDebug() << "Error parse getinfo";
        this->status = ERR_INFO_SYNX;
    } else {
        QJsonObject obj = dom.object();
        if (!obj.contains("name") || !obj.contains("introduction") || !obj.contains("version") ||
            !obj.contains("author") || !obj.contains("keyword") || !obj.contains("icon") || !obj.contains("website") ||
            !obj.contains("CMDmethod")) {
            this->status = ERR_INFO_SYNX;
            qDebug() << "Error info synx";
        } else if (!obj.value("name").isString() || !obj.value("introduction").isString() ||
                   !obj.value("version").isString() || !obj.value("author").isString() ||
                   !obj.value("keyword").isString() || !obj.value("icon").isString() ||
                   !obj.value("website").isString() || !obj.value("CMDmethod").isBool()) {
            this->status = ERR_INFO_SYNX;
            qDebug() << "Error info synx";
        } else {
            ui->name->setText(obj.value("name").toString());
            ui->keyword->setText(obj.value("keyword").toString());
            ui->info->setText(obj.value("introduction").toString());
            ui->version->setText(obj.value("version").toString());
            ui->author->setText(obj.value("author").toString());
            ui->ref->setText("<a href = \"" + obj.value("website").toString() + "\"> open </a>");
            ui->ref->setOpenExternalLinks(true);
            QObject::connect(ui->ref, &QLabel::linkActivated, this, [&](QString s) {
                QProcess p;
                p.setProgram("/usr/bin/xdg-open");
                p.setArguments(QStringList(obj.value("website").toString()));
                ::setuid(1000);
                p.startDetached();
            });
            ui->logo->setPixmap(
                QPixmap(this->path + obj.value("icon").toString()).scaled(100, 100, Qt::KeepAspectRatio));
        }
    }
    if (this->onePlugin->_enable)
        ui->onoff->setText("ON");
    else {
        ui->onoff->setText("OFF");
    }
}
