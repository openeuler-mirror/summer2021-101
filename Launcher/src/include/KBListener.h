//  接收DBus传来的快捷键消息

#ifndef KBLISTENER_H
#define KBLISTENER_H

#include <fcntl.h>
#include <linux/input.h>
#include <stdlib.h>
#include <unistd.h>

#include <QObject>
#include <QSocketNotifier>

#include "errnos.h"

class KBListener : public QObject {
    Q_OBJECT
public:
    KBListener(QObject* parent = NULL);
    ~KBListener();

public slots:
    Q_SCRIPTABLE QString emitShow();

signals:

    void KEY_Show();

private:
};

#endif  // KBLISTENER_H
