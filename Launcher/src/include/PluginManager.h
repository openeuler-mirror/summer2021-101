#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QCloseEvent>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QScrollArea>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "Plugin.h"
#include "PluginManagePanel.h"

namespace Ui {
class PluginManager;
}

class PluginManager : public QMainWindow {
    Q_OBJECT

public:
    explicit PluginManager(QVector<Plugin*>& value, QWidget* parent = nullptr);
    ~PluginManager();

signals:
    void changed();

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::PluginManager* ui;
    QVector<Plugin*>& plugins;

    QVBoxLayout* layout;
    QWidget* contentWidget;

    QVector<QWidget*> contents;

    void setPluginManagePanels();
    void addNewPlugin();
    bool cp_r(QString src, QString dst);
    void refresh();

    int isUpdate(QString name);
};

#endif  // PLUGINMANAGER_H
