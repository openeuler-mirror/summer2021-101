#include "include/KBListener.h"

KBListener::KBListener(QObject *parent) : QObject(parent) {
    this->status = 0;
    this->onScreen = true;
    this->kbP = false;
    this->kbCtrl = false;
    this->kb = open("/dev/input/event3", O_RDWR);
    if (this->kb == -1) {
        perror("open input ERROR");
        this->status = ERR_IO;
    }
    kbNotifer = new QSocketNotifier(this->kb, QSocketNotifier::Read);
    QObject::connect(this->kbNotifer, &QSocketNotifier::activated, this,
                     &KBListener::onKeyPressed);
}

KBListener::~KBListener() {
    this->kbNotifer->deleteLater();
    ::close(this->kb);
}

void KBListener::onKeyPressed(qintptr fd) {
    if (read(fd, &kbEvent, sizeof(kbEvent)) == sizeof(kbEvent)) {
        if (kbEvent.type == EV_KEY) {
            if (kbEvent.value == 1) {
                if (this->onScreen && kbEvent.code == KEY_ESC) {
                    emit KEY_Esc();
                }
                if (this->onScreen && kbEvent.code == KEY_UP) {
                    emit KEY_Up();
                }
                if (this->onScreen && kbEvent.code == KEY_DOWN) {
                    emit KEY_Down();
                }
                if (kbEvent.code == KEY_P) {
                    this->kbP = true;
                }
                if (kbEvent.code == KEY_LEFTMETA ||
                        kbEvent.code == KEY_RIGHTMETA) {
                    this->kbCtrl = true;
                }
            } else {
                if (kbEvent.code == KEY_P) {
                    this->kbP = false;
                }
                if (kbEvent.code == KEY_LEFTMETA ||
                        kbEvent.code == KEY_RIGHTMETA) {
                    this->kbCtrl = false;
                }
            }
        }
        if (this->kbCtrl && this->kbP) {
            emit KEY_Show();
        }
    }
}
