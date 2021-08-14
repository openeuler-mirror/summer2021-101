#include <QApplication>
#include <QDebug>
#include <QDir>
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
    MainWindow w;
    QPalette pal = QPalette();
    pal.setColor(QPalette::Background, QColor(0xA9, 0xA9, 0xA9, 0x50));
    w.setPalette(pal);
    //    w.setWindowFlag(Qt::FramelessWindowHint);

    w.show();

    return a.exec();
}
