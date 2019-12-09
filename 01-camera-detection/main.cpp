#include <thread>
#include <QDebug>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraImageCapture>

int main()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    qDebug() << "Detected cameras:" << cameras.size();

    for (const QCameraInfo &info: cameras) {
        qDebug() << "Opening camera:" << info.deviceName();
        qDebug() << "\t- Description:" << info.description();
        qDebug() << "\t- Position:" << info.position();

        QCamera camera(info);
        QCameraImageCapture capture(&camera);

        camera.load();

        qDebug() << "\t- Camera status ans state:" << camera.status() << camera.state();

        QList<QSize> resolutions = capture.supportedResolutions();
        QList<QVideoFrame::PixelFormat> bufferformats = capture.supportedBufferFormats();
        QStringList imagecodecs = capture.supportedImageCodecs();

        qDebug() << "\t- Resolutions:";
        for (const QSize &resolution : resolutions) {
            qDebug() << "\t\t-" << resolution.width() << "x" << resolution.height();
        }

        qDebug() << "\t- Pixel Formats:";
        for (const QVideoFrame::PixelFormat &format : bufferformats) {
            qDebug() << "\t\t-" << format;
        }

        qDebug() << "\t- Image Codecs:";
        for (const QString &codec : imagecodecs) {
            qDebug() << "\t\t-" << codec << capture.imageCodecDescription(codec);
        }

        /*QImageEncoderSettings settings = capture.encodingSettings();
        settings.setCodec(imagecodecs[0]);
        settings.setQuality(QMultimedia::EncodingQuality::VeryHighQuality);
        settings.setResolution(resolutions[0]);
        capture.setEncodingSettings(settings);*/
    }

    return 0;
}
