#include "include/PluginPanel.h"

#include "ui_PluginPanel.h"

PluginPanel::PluginPanel(QString basePath, QString icon, QString cmd, QStringList argvs, QString info, int index,
                         QWidget *parent)
    : QWidget(parent), ui(new Ui::PluginPanel) {
    ui->setupUi(this);
    this->argvs = argvs;
    this->index = index;
    qDebug() << basePath + "/" + icon;
    pic0 = new QPixmap(QDir::currentPath() + "/plugins/Go.png");
    pic1 = new QPixmap(QDir::currentPath() + "/plugins/go.png");

    ui->label->setPixmap(QPixmap(basePath + "/" + icon));
    ui->labelCmd->setText(cmd);
    ui->labelArgvs->setText(argvs.join(" "));
    ui->labelGo->setPixmap(QPixmap(QDir::currentPath() + "/plugins/Go.png"));
    ui->labelInfo->setText(info);

    //    this->setStyleSheet("QWidget{color:white;spacing:2px;min-height:28px;min-width:60px;border:2px solid rgb(255,
    //    87, 90,55)}");
}

PluginPanel::~PluginPanel() { delete ui; }

void PluginPanel::selected(bool flag) {
    if (flag) {
        ui->labelGo->setPixmap(*this->pic0);
    } else {
        ui->labelGo->setPixmap(*this->pic1);
    }
}

void PluginPanel::mouseReleaseEvent(QMouseEvent *ev) { emit click(this->index); }

QStringList PluginPanel::getArgvs() const { return argvs; }
