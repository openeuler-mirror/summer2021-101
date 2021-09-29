#include <QCoreApplication>
#include <QObject>
#include <QtDBus/QtDBus>

#define SERVICE_NAME "org.QtDBus.Launcher"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
        return -1;
    }

    QDBusInterface iface(SERVICE_NAME, "/", "", QDBusConnection::sessionBus());
    if (iface.isValid()) {
        QDBusReply<QString> reply = iface.call("emitShow");
        if (reply.isValid()) {
            printf("emitShow ok");
            return 0;
        }

        fprintf(stderr, "Call failed: %s\n", qPrintable(reply.error().message()));
        return 1;
    }

    fprintf(stderr, "%s\n", qPrintable(QDBusConnection::sessionBus().lastError().message()));

    return 1;
}
