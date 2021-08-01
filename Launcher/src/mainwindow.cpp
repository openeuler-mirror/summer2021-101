#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    resetWindowSize();
    setKBListener();

    this->conf = new MainConfig("/home/miao/Documents/github/summer2021-101/Launcher/conf.json");
    this->plugins = this->conf->getPlugins();

    this->contentWidget = new QWidget();
    this->layout = new QVBoxLayout();
    ui->scrollArea->setWidget(this->contentWidget);
    this->layout->setAlignment(Qt::AlignTop);
    this->contentWidget->resize(1024, 1024);
    this->contentWidget->setLayout(this->layout);

    QObject::connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::inHandler);
    QObject::connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::selectHandler);
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
    ui->verticalSpacer->changeSize(0, height);
}

void MainWindow::setKBListener() {
    this->kbListener = new KBListener();

    QObject::connect(this->kbListener, &KBListener::KEY_Esc, this, [&]() {
        //        this->hide();
        this->close();
        this->kbListener->onScreen = false;
    });
    QObject::connect(this->kbListener, &KBListener::KEY_Show, this, [&]() {
        this->show();
        this->kbListener->onScreen = true;
    });

    QObject::connect(this->kbListener, &KBListener::KEY_Up, this, [&]() {
        this->selected = (this->selected - 1) % this->matched.count();
        qDebug() << this->selected;
    });

    QObject::connect(this->kbListener, &KBListener::KEY_Down, this, [&]() {
        this->selected = (this->selected + 1) % this->matched.count();
        qDebug() << this->selected;
    });
}

void MainWindow::inHandler(const QString& text) {
    qDebug() << text;
    for (int i = 0; i < this->layout->count(); ++i) {
        this->layout->removeItem(this->layout->itemAt(0));
    }

    for (int i = 0; i < this->contents.length(); ++i) {
        this->contents.at(i)->setParent(nullptr);
        this->contents.at(i)->deleteLater();
    }
    this->contents.clear();

    this->matched.clear();

    for (int i = 0; i < this->plugins.count(); ++i) {
        if (text.compare(this->plugins.at(i)->_keyword) == 0) {
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
            if (obj.contains("icon") && obj.contains("cmd") && obj.contains("argvs") && obj.contains("info")) {
                if (obj.value("icon").isString() && obj.value("cmd").isString() && obj.value("argvs").isArray() &&
                    obj.value("info").isString()) {
                    QStringList args;
                    QJsonArray argvs = obj.value("argvs").toArray();
                    for (int j = 0; j < argvs.count(); ++j) {
                        args << argvs.at(j).toString();
                    }

                    PluginPanel* pp = new PluginPanel(QDir::currentPath() + "/plugins/" + this->plugins.at(i)->_name,
                                                      obj.value("icon").toString(), obj.value("cmd").toString(), args,
                                                      obj.value("info").toString(), this->matched.at(i));
                    qDebug() << obj.value("icon").toString() << obj.value("cmd").toString() << args
                             << obj.value("info").toString();
                    this->contents.push_back(pp);
                }
            }
        }
    }

    //
    for (int i = 0; i < this->contents.length(); ++i) {
        this->layout->addWidget(this->contents.at(i));
    }
    this->selected = 0;
}

void MainWindow::selectHandler() {
    PluginPanel* pp = (PluginPanel*)this->contents.at(this->matched.at(this->selected));
    Plugin* pl = this->plugins.at(this->matched.at(this->selected));

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
        ::setuid(1000);
        p.setWorkingDirectory(QDir::currentPath() + "/plugins/" + pl->_name);
        this->hide();
        p.startDetached();
        qDebug() << "Process started";
    }
}
