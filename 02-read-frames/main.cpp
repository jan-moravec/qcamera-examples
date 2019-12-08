#include <thread>
#include <QDebug>
#include <QCamera>
#include <QCameraInfo>
#include "videosurface.h"

int main()
{
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

    camera.start();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    camera.stop();

    return 0;
}
