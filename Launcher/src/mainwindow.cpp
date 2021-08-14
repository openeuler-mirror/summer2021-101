#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    resetWindowSize();
    setKBListener();
    this->pluginManager = NULL;

    this->conf = new MainConfig("/home/miao/Documents/github/summer2021-101/Launcher/conf.json", plugins);
    //    this->plugins = this->conf->getPlugins();

    this->contentWidget = new QWidget();
    this->layout = new QVBoxLayout();
    ui->scrollArea->setWidget(this->contentWidget);
    this->layout->setAlignment(Qt::AlignTop);
    this->contentWidget->resize(1024, 1024);
    this->contentWidget->setLayout(this->layout);

    QObject::connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::inHandler);
    QObject::connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::selectHandler);
    QObject::connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::setManagerPage);
    this->selected = 0;
}

MainWindow::~MainWindow() {
    delete ui;
    this->kbListener->deleteLater();
}

void MainWindow::resetWindowSize() {
    this->setWindowOpacity(0.9);
    QRect size = QGuiApplication::primaryScreen()->availableGeometry();
    this->setGeometry(size);
    int width = size.width() / 4;
    int height = size.height() / 4;
    qDebug() << "Window size:widht" << size.width() << "height" << size.height();
    ui->horizontalSpacer->changeSize(width, 0);
    ui->horizontalSpacer_2->changeSize(width, 0);
    //    ui->verticalSpacer->changeSize(0, height);
}

void MainWindow::setKBListener() {
    this->kbListener = new KBListener();

    QObject::connect(this->kbListener, &KBListener::KEY_Esc, this, [&]() {
        ui->lineEdit->clear();
        this->hide();

        //        this->close();
        this->kbListener->onScreen = false;
    });
    QObject::connect(this->kbListener, &KBListener::KEY_Show, this, [&]() {
        this->show();
        this->kbListener->onScreen = true;
    });

    QObject::connect(this->kbListener, &KBListener::KEY_Up, this, [&]() {
        if (this->contents.isEmpty()) {
            return;
        }
        ((PluginPanel*)(this->contents.at(this->selected)))->selected(false);
        this->selected = (this->selected + this->map.count() - 1) % this->map.count();
        ((PluginPanel*)(this->contents.at(this->selected)))->selected(true);
    });

    QObject::connect(this->kbListener, &KBListener::KEY_Down, this, [&]() {
        if (this->contents.isEmpty()) {
            return;
        }
        ((PluginPanel*)(this->contents.at(this->selected)))->selected(false);
        this->selected = (this->selected + this->map.count() + 1) % this->map.count();
        ((PluginPanel*)(this->contents.at(this->selected)))->selected(true);
    });
}

void MainWindow::setManagerPage() {
    ui->lineEdit->clear();
    this->hide();
    this->pluginManager = new PluginManager(this->plugins, this);
    this->pluginManager->show();
    QObject::connect(this->pluginManager, &PluginManager::changed, this->conf, &MainConfig::reWrite);
    qDebug() << "[Info] PluginManager: open";
}

void MainWindow::inHandler(const QString& text) {
    qDebug() << text;
    QStringList inList = text.split(" ");
    for (int i = 0; i < this->layout->count(); ++i) {
        this->layout->removeItem(this->layout->itemAt(0));
    }

    for (int i = 0; i < this->contents.length(); ++i) {
        this->contents.at(i)->setParent(nullptr);
        this->contents.at(i)->deleteLater();
    }
    this->contents.clear();

    this->matched.clear();

    this->map.clear();

    for (int i = 0; i < this->plugins.count(); ++i) {
        if (inList.at(i).compare(this->plugins.at(i)->_keyword) == 0) {
            this->matched.push_back(i);
        }
    }

    QJsonDocument dom;
    QJsonParseError jsErr;

    for (int i = 0; i < this->matched.count(); ++i) {
        QString query =
            QString::fromStdString(this->plugins.at(this->matched.at(i))->plugin->preExec(text.toStdString()));
        qDebug() << query;
        dom = QJsonDocument::fromJson(query.toLocal8Bit(), &jsErr);
        if (dom.isNull() || jsErr.error != QJsonParseError::NoError || !dom.isObject()) {
            continue;
        } else {
            QJsonObject obj = dom.object();
            if (obj.contains("icon") && obj.contains("panels")) {
                if (obj.value("icon").isString() && obj.value("panels").isArray()) {
                    QString icon = obj.value("icon").toString();
                    QJsonArray array = obj.value("panels").toArray();
                    for (int j = 0; j < array.count(); ++j) {
                        if (!array.at(j).isObject()) {
                            continue;
                        } else {
                            QJsonObject objPanel = array.at(j).toObject();
                            if (!objPanel.contains("cmd") || !objPanel.contains("argvs") ||
                                !objPanel.contains("info")) {
                                continue;

                            } else {
                                if (!objPanel.value("cmd").isString() || !objPanel.value("argvs").isArray() ||
                                    !objPanel.value("info").isString()) {
                                    continue;
                                } else {
                                    QStringList args;
                                    QJsonArray argvs = objPanel.value("argvs").toArray();
                                    for (int j = 0; j < argvs.count(); ++j) {
                                        args << argvs.at(j).toString();
                                    }

                                    PluginPanel* pp = new PluginPanel(
                                        QDir::currentPath() + "/plugins/" + this->plugins.at(i)->_name,
                                        this->plugins.at(i)->_name, icon, objPanel.value("cmd").toString(), args,
                                        objPanel.value("info").toString(), this->map.count());
                                    this->map.push_back(i);
                                    qDebug() << obj.value("icon").toString() << obj.value("cmd").toString() << args
                                             << obj.value("info").toString();
                                    this->contents.push_back(pp);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //
    for (int i = 0; i < this->contents.length(); ++i) {
        this->layout->addWidget(this->contents.at(i));
    }
    this->selected = 0;
    if (!this->contents.isEmpty()) {
        ((PluginPanel*)(this->contents.at(this->selected)))->selected(true);
    }
    qDebug() << "map:" << this->map;
}

void MainWindow::selectHandler() {
    if (this->contents.count() == 0) {
        ui->lineEdit->clear();
        this->hide();
        return;
    }

    PluginPanel* pp = (PluginPanel*)this->contents.at(this->selected);
    Plugin* pl = this->plugins.at(this->matched.at(this->map.at(this->selected)));

    QString ret = QString::fromStdString(pl->plugin->realExec(pp->getArgvs().join(" ").toStdString()));

    QProcess p;
    QStringList args;

    QJsonDocument dom;
    QJsonParseError jsErr;
    bool status = false;

    if (pl->cmd) {
        dom = QJsonDocument::fromJson(ret.toLocal8Bit(), &jsErr);
        if (dom.isNull() || jsErr.error != QJsonParseError::NoError || !dom.isObject()) {
            qDebug() << "Error parse realExec";
        } else {
            QJsonObject obj = dom.object();
            if (!obj.contains("status") || !obj.contains("cmd") || !obj.contains("argvs")) {
                qDebug() << "readexec ret error";
            } else {
                if (!obj.value("status").isBool() || !obj.value("cmd").isString() || !obj.value("argvs").isArray()) {
                    qDebug() << "readexec ret error";
                } else {
                    if (obj.value("status").toBool()) {
                        QString cmd = obj.value("cmd").toString();
                        if (!QDir::isAbsolutePath(cmd)) {
                            cmd = QDir::currentPath() + "/plugins/" + pl->_name + "/" + cmd;
                        }

                        p.setProgram(cmd);
                        qDebug() << "dir " << QDir::currentPath() + "/plugins/" + pl->_name;
                        //                        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
                        //                        env.insert("PLUGINDIR",
                        //                                   QDir::currentPath() + "/plugins/" + pl->_name);  // Add an
                        //                                   environment variable
                        //                        p.setProcessEnvironment(env);

                        QJsonArray argvs = obj.value("argvs").toArray();
                        for (int j = 0; j < argvs.count(); ++j) {
                            args << argvs.at(j).toString();
                        }

                        qDebug() << "cmd:" << cmd << "args" << args;
                        p.setArguments(args);
                        status = true;
                    }
                }
            }
        }
    }

    if (status) {
        p.setWorkingDirectory(QDir::currentPath() + "/plugins/" + pl->_name);
        qDebug() << p.workingDirectory();

        p.startDetached();
        qDebug() << "Process started";
        qDebug() << p.state();
        ui->lineEdit->clear();
        this->hide();
    }
}

void MainWindow::showEvent(QCloseEvent* event) { ui->lineEdit->setFocus(); }
