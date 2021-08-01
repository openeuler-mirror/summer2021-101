#ifndef PLUGINPANEL_H
#define PLUGINPANEL_H

#include <QDebug>
#include <QDir>
#include <QWidget>

namespace Ui {
class PluginPanel;
}

class PluginPanel : public QWidget {
    Q_OBJECT

signals:
    void click(int i);

public:
    explicit PluginPanel(QString basePath, QString icon, QString cmd, QStringList argvs, QString info, int index,
                         QWidget *parent = nullptr);
    ~PluginPanel();
    void selected(bool flag);

    QStringList getArgvs() const;

protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev);

private:
    Ui::PluginPanel *ui;
    QFont *font;
    QStringList argvs;
    int index;

    QPixmap *pic0;
    QPixmap *pic1;
};

#endif  // PLUGINPANEL_H
