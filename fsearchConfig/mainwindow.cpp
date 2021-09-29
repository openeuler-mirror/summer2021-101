#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QDir::setCurrent("/home/miao/Documents/github/summer2021-101/Launcher/");

    fp.setFileName(QDir::currentPath() + "/plugins/fsearch/dirs/searchDirs.txt");

    qDebug() << fp.fileName();
    if (!fp.open(QIODevice::ReadWrite | QIODevice::Text)) {
        ui->textEdit->setReadOnly(true);
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
    }

    while (!fp.atEnd()) {
        QByteArray line = fp.readLine().trimmed();

        ui->textEdit->append(QString(line));
    }

    QObject::connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onCancel);
    QObject::connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onConfirm);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::onCancel() {
    this->fp.close();
    this->close();
}

void MainWindow::onConfirm() {
    fp.resize(0);
    fp.write(ui->textEdit->toPlainText().toLocal8Bit());
    fp.close();
    this->close();
}
