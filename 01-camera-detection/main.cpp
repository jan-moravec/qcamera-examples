#include <QDebug>
#include <QCamera>
#include <QCameraInfo>

int main()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    qDebug() << "Detected cameras:" << cameras.size();

    for (const QCameraInfo &info: cameras) {
        qDebug() << "Opening camera:" << info.deviceName();
        qDebug() << "\t- Description:" << info.description();
        qDebug() << "\t- Position:" << info.position();

        QCamera camera(info);
        QList<QSize> resolutions = camera.supportedViewfinderResolutions();
        QList<QVideoFrame::PixelFormat> pixelformats = camera.supportedViewfinderPixelFormats();
        QList<QCamera::FrameRateRange> framerates = camera.supportedViewfinderFrameRateRanges();

        qDebug() << "\t- Resolutions:";
        for (const QSize &resolution : resolutions) {
            qDebug() << "\t\t-" << resolution.width() << "x" << resolution.height();
        }

        qDebug() << "\t- Pixel Formats:";
        for (const QVideoFrame::PixelFormat &format : pixelformats) {
            qDebug() << "\t\t-" << format;
        }

        qDebug() << "\t- Frame Rates:";
        for (const QCamera::FrameRateRange &rate : framerates) {
            qDebug() << "\t\t-" << rate.minimumFrameRate << "to" << rate.maximumFrameRate;
        }

        //QCameraViewfinderSettings settings;
        //settings.setResolution(1920, 1080);
        //camera.setViewfinderSettings(settings);
    }

    return 0;
}
