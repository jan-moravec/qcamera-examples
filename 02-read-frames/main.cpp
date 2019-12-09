#include <QCoreApplication>
#include <QDebug>
#include <QCamera>
#include <QCameraInfo>
#include <QTimer>
#include "videosurface.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (!QCameraInfo::availableCameras().size()) {
        qDebug() << "No cameras connected.";
        return 0;
    }

    QCamera camera;
    if (!camera.isAvailable()) {
        qDebug() << "Camera not available.";
        return 0;
    }

    VideoSurface surface;
    camera.setViewfinder(&surface);

    //camera.load();
    camera.start();

    QTimer::singleShot(5000, &a, QCoreApplication::quit);

    return a.exec();
}
