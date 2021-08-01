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
#include <QMainWindow>
#include <QProcess>
#include <QProcessEnvironment>
#include <QPushButton>
#include <QRect>
#include <QScreen>
#include <QSocketNotifier>

#include "KBListener.h"
#include "MainConfig.h"
#include "Plugin.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void inHandler(const QString &text);
    void selectHandler();

private:
    Ui::MainWindow *ui;

    KBListener *kbListener;

    MainConfig *conf;
    QVector<Plugin *> plugins;
    QVBoxLayout *layout;
    QWidget *contentWidget;

    QVector<int> matched;
    QVector<QWidget *> contents;

    int selected;

    void resetWindowSize();
    void setKBListener();
    //    void KBListener(qintptr fd);
};
#endif  // MAINWINDOW_H
