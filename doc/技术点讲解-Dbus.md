# Dbus

## 简介

DBus是一种桌面环境的进程间通信(IPC)机制，低延迟，低开销，高可用

官网：https://dbus.freedesktop.org/

官方教程：https://dbus.freedesktop.org/doc/dbus-tutorial.html

DBus介绍：https://www.freedesktop.org/wiki/IntroductionToDBus/

分为以下三个层次：

libdbus：库

dbus-daemon：守护进程，提供dbus的建立、消息转发

定制：如GTK封装的GDBus、如QT封装的QtDBus

## 相关概念

![img](https://dbus.freedesktop.org/doc/diagram.png)

### 总线

公交车，人来人往，有上有下。

网络总线，数据包传输，从某个IP发出，到某个IP接收。

Dbus总线，进程间通信，某个进程发起，某个进程接收

通常可以认为有两类总线：

系统总线和会话总线，系统总线为系统公用，会话总线为某个会话专属使用



### 本地对象与对象路径

为了能够访问到本地对象，对象有一个自己的对象路径，包含类似域名、路径等信息，比如/home/miao

### 方法与信号

对于对象的调用，可以调用一个方法，可以触发一个信号。方法有输入和输出，信号也可以有数据载荷。

### 接口

每个对象支持一个或者多个接口，将接口视为一组命名的方法和信号。Dbus采用如下字符串来标识如`org.freedesktop.dbusexample`

大多将接口映射到Java接口或者C++纯虚类

### 对象

总线上任何交换的一端为端点，Dbus中称为对象，一个客户端进程能够创建多个对象



### 代理

总线上的对象可以通过我们称为*代理的*引用进行访问*。* 我们之所以这样称呼它们，是因为代理是您自己的程序内部对象的本地表示，该对象实际上是通过总线访问的，并且通常位于您的程序之外。



### 总线名字

**unique**和**well-known**唯一的总线名称由总线分配给每个客户端连接。它们以*:*开头，并且保证它们在总线的生命周期内永远不会被重复使用。与可以翻转和重用的进程 ID 不同，唯一的总线名称保证是真正唯一的。

D-Bus 客户可以请求附加的“知名”总线名称，以便以约定俗成的名称提供其服务。这允许应用程序在已知位置轻松找到所提供的服务。例如，Gnome 网络管理器在众所周知的*org.freedesktop.NetworkManager*总线名称上提供其服务，以防止潜在用户需要确定网络管理器的唯一总线名称。

### 总的来看

总体来看：地址 -> [总线名称] -> 路径 -> 接口 -> 方法

举例：

- 地址：unix:path=/var/run/dbus/system_bus_socket
- 名字：org.freedesktop.TextEditor（：15-165）
- 路径：/home/miao
- 接口：com.mycompany.TextEditor
- 方法：getName

## QtDBus

简单例子，可以在QT的示例中找到pingpong项目

ping.h

```c++
#ifndef PONG_H
#define PONG_H

#include <QtCore/QObject>

class Pong: public QObject
{
    Q_OBJECT
public slots:
    Q_SCRIPTABLE QString ping(const QString &arg);
};

#endif

```

pong.cpp

```C++
#include <stdio.h>
#include <stdlib.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtDBus/QtDBus>

#define SERVICE_NAME            "org.example.QtDBus.PingExample"
#include "pong.h"

QString Pong::ping(const QString &arg)
{
    QMetaObject::invokeMethod(QCoreApplication::instance(), "quit");
    return QString("ping(\"%1\") got called").arg(arg);
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
        fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                "To start it, run:\n"
                "\teval `dbus-launch --auto-syntax`\n");
        return 1;
    }

    if (!QDBusConnection::sessionBus().registerService(SERVICE_NAME)) {
        fprintf(stderr, "%s\n",
                qPrintable(QDBusConnection::sessionBus().lastError().message()));
        exit(1);
    }

    Pong pong;
    QDBusConnection::sessionBus().registerObject("/", &pong, QDBusConnection::ExportAllSlots);

    app.exec();
    return 0;
}

```

可以看到，此进程在DBus注册了本地服务QString Pong::ping(const QString &arg);

pong进程由此进入循环等待连接

ping.cpp

```C++

#include <stdio.h>

#include <QtCore/QCoreApplication>
#include <QtDBus/QtDBus>

#define SERVICE_NAME            "org.example.QtDBus.PingExample"

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
        fprintf(stderr,
                "Cannot connect to the D-Bus session bus.\n"
                "To start it, run:\n"
                "\teval `dbus-launch --auto-syntax`\n");
        return 1;
    }

    QDBusInterface iface(SERVICE_NAME, "/", "", QDBusConnection::sessionBus());
    if (iface.isValid()) {
        QDBusReply<QString> reply = iface.call("ping", argc > 1 ? argv[1] : "");
        if (reply.isValid()) {
            printf("Reply was: %s\n", qPrintable(reply.value()));
            return 0;
        }

        fprintf(stderr, "Call failed: %s\n", qPrintable(reply.error().message()));
        return 1;
    }

    fprintf(stderr, "%s\n", qPrintable(QDBusConnection::sessionBus().lastError().message()));
    return 1;
}

```

可以看到，ping进程连接到DBus并通过name、path等调用ping服务，pong进程中会调用其中的ping函数并执行退出。



