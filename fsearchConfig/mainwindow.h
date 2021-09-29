#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onCancel();
    void onConfirm();

private:
    Ui::MainWindow *ui;
    QFile fp;
};
#endif  // MAINWINDOW_H
