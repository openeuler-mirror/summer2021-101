#ifndef PLUGINMANAGEPANEL_H
#define PLUGINMANAGEPANEL_H

#include <QDesktopServices>
#include <QPixmap>
#include <QProcess>
#include <QUrl>
#include <QWidget>

#include "Plugin.h"
#include "errnos.h"

namespace Ui {
class PluginManagePanel;
}

class PluginManagePanel : public QWidget {
    Q_OBJECT

public:
    explicit PluginManagePanel(Plugin *onePlugin, QWidget *parent = nullptr);
    ~PluginManagePanel();

    int status;

private:
    Ui::PluginManagePanel *ui;
    Plugin *onePlugin;
    QString path;

    bool ifConfig = false;
    QString config;

    void setupInfo();
};

#endif  // PLUGINMANAGEPANEL_H
