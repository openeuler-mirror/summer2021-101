#include "mainwindow.h"
#include <QPalette>
#include <QLabel>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QApplication>
#include <QDebug>
#include <QRect>
#include <QDir>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QPalette pal = QPalette();
    pal.setColor(QPalette::Background, QColor(0xA9,0xA9,0xA9,0x50));
    w.setPalette(pal);
//    w.setWindowFlag(Qt::FramelessWindowHint);




    w.show();




    return a.exec();



//    QApplication a(argc, argv);
//        QWidget w;
//        QLabel *label = new QLabel(&w);
//        QImage image("D:\\QT\\ScrollArea\\1.jpg");
//        QHBoxLayout *lay=new QHBoxLayout(&w);
//        label->setPixmap(QPixmap::fromImage(image));
//        //创建滚动区，
//        QScrollArea *scrollarea = new QScrollArea(&w);
//        //将Widget组件加入滚动区
//        scrollarea->setWidget(label);
//        //自动改变滚动区大小
//        scrollarea->setWidgetResizable(1);
//        //设置滚动区背景颜色
//        scrollarea->setBackgroundRole(QPalette::Dark);
//        lay->addWidget(scrollarea);
//        w.setLayout(lay);
//        w.show();
//        return a.exec();
}
