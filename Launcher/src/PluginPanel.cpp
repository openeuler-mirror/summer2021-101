#include "include/PluginPanel.h"

#include "ui_PluginPanel.h"

PluginPanel::PluginPanel(QString basePath, QString name, QString icon, QString cmd, QStringList argvs, QString info,
                         int index, QWidget *parent)
    : QWidget(parent), ui(new Ui::PluginPanel) {
    setAttribute(Qt::WA_StyledBackground, true);
    ui->setupUi(this);
    this->argvs = argvs;
    this->index = index;
    pic0 = new QPixmap(QDir::currentPath() + "/plugins/go.png");
    pic1 = new QPixmap(QDir::currentPath() + "/plugins/Go.png");

    ui->label->setPixmap(QPixmap(basePath + "/" + icon).scaled(100, 100, Qt::KeepAspectRatio));
    ui->labelName->setText(name);
    ui->labelCmd->setText(cmd);
    ui->labelArgvs->setText(argvs.join(" "));
    ui->labelGo->setPixmap((*pic1).scaled(100, 100, Qt::KeepAspectRatio));
    ui->labelInfo->setText(info);
}

PluginPanel::~PluginPanel() { delete ui; }

void PluginPanel::selected(bool flag) {
    if (flag) {
        ui->labelGo->setPixmap((*pic0).scaled(100, 100, Qt::KeepAspectRatio));
    } else {
        ui->labelGo->setPixmap((*pic1).scaled(100, 100, Qt::KeepAspectRatio));
    }
}

void PluginPanel::mouseReleaseEvent(QMouseEvent *ev) { emit click(this->index); }

QStringList PluginPanel::getArgvs() const { return argvs; }
