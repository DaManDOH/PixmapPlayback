#include "mainwindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlag(Qt::Dialog);
    w.setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    w.showMaximized();
    return a.exec();
}
