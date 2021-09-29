#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <PluginPanel.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdlib.h>
#include <unistd.h>

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QList>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QProcessEnvironment>
#include <QPushButton>
#include <QRect>
#include <QScreen>
#include <QScrollBar>
#include <QShowEvent>
#include <QSocketNotifier>
#include <QtDBus>

#include "KBListener.h"
#include "MainConfig.h"
#include "Plugin.h"
#include "PluginManager.h"
#include "errnos.h"

#define SERVICE_NAME "org.QtDBus.Launcher"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
public slots:
    void inHandler(const QString& text);
    void selectHandler();

protected:
    virtual void showEvent(QCloseEvent* event);
    void keyPressEvent(QKeyEvent* event);

private:
    Ui::MainWindow* ui;

    // 监控键盘快捷键
    KBListener* kbListener;

    MainConfig* conf;
    // 指向各个插件类
    QVector<Plugin*> plugins;

    QVBoxLayout* layout;

    QWidget* contentWidget;

    // 指向输入匹配到的插件
    QVector<int> matched;

    // 生成所有候选插件面板
    QVector<QWidget*> contents;

    // 存放候选面板对应的match的第几个
    QList<int> map;

    int selected;
    PluginManager* pluginManager;

    void resetWindowSize();
    void setKBListener();
    void setManagerPage();
    //    void KBListener(qintptr fd);
};
#endif  // MAINWINDOW_H
