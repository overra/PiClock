#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QHostInfo>

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(resources);
    QApplication a(argc, argv);
    MainWindow w;

    qDebug() << QHostInfo::localHostName();

    if( QHostInfo::localHostName() != "johanns-imac")
    {
        w.setWindowState(Qt::WindowFullScreen);
    }
    w.show();

    return a.exec();
}
