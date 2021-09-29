#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QPalette>
#include <QRect>
#include <QScrollArea>
#include <QVector>

#include "Plugin.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    //    ::daemon(1, 0);

    QApplication a(argc, argv);

    QFile styleFile(":a.qss");
    if (styleFile.open(QIODevice::ReadOnly)) {
        a.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }

    MainWindow w;
    w.show();

    return a.exec();
}
