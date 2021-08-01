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
    int status;
    explicit KBListener(QObject* parent = nullptr);
    ~KBListener();
    bool onScreen;

signals:
    void KEY_Esc();
    void KEY_Show();
    void KEY_Close();
    //    void KEY_Hide();
    void KEY_Up();
    void KEY_Down();

private:
    int kb;

    bool kbCtrl;
    bool kbP;

    QSocketNotifier* kbNotifer;
    struct input_event kbEvent;
    void onKeyPressed(qintptr fd);
};

#endif  // KBLISTENER_H
