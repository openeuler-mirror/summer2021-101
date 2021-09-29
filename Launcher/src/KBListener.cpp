#include "include/KBListener.h"

KBListener::KBListener(QObject *parent) : QObject(parent) {}

KBListener::~KBListener() {}

QString KBListener::emitShow() {
    emit KEY_Show();
    return "OK";
}
