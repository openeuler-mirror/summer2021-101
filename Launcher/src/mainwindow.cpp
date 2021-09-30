#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    setAttribute(Qt::WA_StyledBackground, true);
    ui->setupUi(this);
    QDir::setCurrent(QDir::homePath() + "/.Launcher/Launcher");

    qDebug() << "[Info]: Working folder:" << QDir::currentPath();

    resetWindowSize();
    setKBListener();
    this->pluginManager = NULL;

    this->conf = new MainConfig("conf.json", plugins);
    if (this->conf->getStatus() != 0) {
        qDebug() << "[Error]: Main config read error with status " << this->conf->getStatus();

        exit(0);
        //        QMessageBox::critical(nullptr, "Error", "Config Error");

    } else {
        qDebug() << "[Info]: Main config read OK!";
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
    ui->horizontalSpacer->changeSize(width, 0);
    ui->horizontalSpacer_2->changeSize(width, 0);
}

void MainWindow::setKBListener() {
    this->kbListener = new KBListener();
    QObject::connect(this->kbListener, &KBListener::KEY_Show, this, [&]() {
        this->show();
        this->setWindowState(Qt::WindowActive);
        this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        ui->lineEdit->setFocus();
    });
    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "[Error]: Connect to DBus error:" << QDBusConnection::sessionBus().lastError().message();
        this->close();
    }

    if (!QDBusConnection::sessionBus().registerService(SERVICE_NAME)) {
        qDebug() << "[Error]: Rigister DBus service error:" << QDBusConnection::sessionBus().lastError().message();
        this->close();
    }
    QDBusConnection::sessionBus().registerObject("/", this->kbListener, QDBusConnection::ExportAllSlots);
}

void MainWindow::setManagerPage() {
    ui->lineEdit->clear();
    this->hide();
    this->pluginManager = new PluginManager(this->plugins, this);
    this->pluginManager->show();
    QObject::connect(this->pluginManager, &PluginManager::changed, this->conf, &MainConfig::reWrite);

    //    for (int i = 0; i < this->plugins.size(); ++i) {
    //        this->plugins.at(i)->plugin->update();
    //    }
}

void MainWindow::inHandler(const QString& text) {
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
        if (inList.at(0).compare(this->plugins.at(i)->_keyword) == 0) {
            this->matched.push_back(i);
        }
    }

    inList.removeAt(0);
    QString oneSearch = inList.join(" ");

    QJsonDocument dom;
    QJsonParseError jsErr;

    for (int i = 0; i < this->matched.count(); ++i) {
        QString query =
            QString::fromStdString(this->plugins.at(this->matched.at(i))->plugin->preExec(oneSearch.toStdString()));
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
                                        QDir::currentPath() + "/plugins/" +
                                            this->plugins.at(this->matched.at(i))->_name,
                                        this->plugins.at(i)->_name, icon, objPanel.value("cmd").toString(), args,
                                        objPanel.value("info").toString(), this->map.count());
                                    this->map.push_back(i);
                                    //                                    qDebug() << "[Info]: Generate one Panel:" <<
                                    //                                    obj.value("icon").toString()
                                    //                                             << obj.value("cmd").toString() <<
                                    //                                             args << obj.value("info").toString();
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
            qDebug() << "[Info]: RealExec Parse failed";
        } else {
            QJsonObject obj = dom.object();
            if (!obj.contains("status") || !obj.contains("cmd") || !obj.contains("argvs")) {
                qDebug() << "[Info]: RealExec Syntax Error";
            } else {
                if (!obj.value("status").isBool() || !obj.value("cmd").isString() || !obj.value("argvs").isArray()) {
                    qDebug() << "[Info]: RealExec Syntax Error";
                } else {
                    if (obj.value("status").toBool()) {
                        QString cmd = obj.value("cmd").toString();
                        if (!QDir::isAbsolutePath(cmd)) {
                            cmd = QDir::currentPath() + "/plugins/" + pl->_name + "/res/" + cmd;
                        }
                        p.setProgram(cmd);
                        //                        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
                        //                        env.insert("PLUGINDIR",
                        //                                   QDir::currentPath() + "/plugins/" + pl->_name);  // Add an
                        //                                   environment variable
                        //                        p.setProcessEnvironment(env);

                        QJsonArray argvs = obj.value("argvs").toArray();
                        for (int j = 0; j < argvs.count(); ++j) {
                            args << argvs.at(j).toString();
                        }
                        p.setArguments(args);
                        status = true;
                    }
                }
            }

            if (status) {
                p.setWorkingDirectory(QDir::currentPath() + "/plugins/" + pl->_name);
                p.startDetached();
                ui->lineEdit->clear();
                this->hide();
            }
        }
    } else {
        ui->lineEdit->clear();
        this->hide();
    }
}

void MainWindow::showEvent(QCloseEvent* event) { ui->lineEdit->setFocus(); }

void MainWindow::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Escape:
        ui->lineEdit->clear();
        this->hide();

        //        this->close();

        break;

    case Qt::Key_Up:
        if (this->contents.isEmpty()) {
            return;
        }
        ((PluginPanel*)(this->contents.at(this->selected)))->selected(false);
        this->selected = (this->selected + this->map.count() - 1) % this->map.count();
        ((PluginPanel*)(this->contents.at(this->selected)))->selected(true);

        ui->scrollArea->verticalScrollBar()->setValue((this->selected) * 1048 / this->contents.length());

        break;

    case Qt::Key_Down:
        if (this->contents.isEmpty()) {
            return;
        }
        ((PluginPanel*)(this->contents.at(this->selected)))->selected(false);
        this->selected = (this->selected + this->map.count() + 1) % this->map.count();
        ((PluginPanel*)(this->contents.at(this->selected)))->selected(true);

        ui->scrollArea->verticalScrollBar()->setValue((this->selected) * 1048 / this->contents.length());

        break;

    default:
        QWidget::keyPressEvent(event);
    }
}
