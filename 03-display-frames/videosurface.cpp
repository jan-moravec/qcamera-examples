#include "videosurface.h"
#include <QDebug>
#include <QDateTime>

VideoSurface::VideoSurface()
{

}

VideoSurface::~VideoSurface()
{

}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType) const
{
    QList<QVideoFrame::PixelFormat> formats;

    for (int i = static_cast<int>(QVideoFrame::PixelFormat::Format_ARGB32); i < static_cast<int>(QVideoFrame::PixelFormat::Format_AdobeDng); ++i) {
        formats << static_cast<QVideoFrame::PixelFormat>(i);
    }

    return formats;
}

bool VideoSurface::present(const QVideoFrame &frame)
{
    qDebug() << QDateTime::currentDateTime().toString() << counter++ << frame.width() << frame.height() << frame.pixelFormat();

    if (frame.mapMode() == QAbstractVideoBuffer::MapMode::ReadOnly || frame.mapMode() == QAbstractVideoBuffer::MapMode::ReadWrite) {
        process(frame);
    } else {
        QVideoFrame local = frame;
        if (!local.map(QAbstractVideoBuffer::MapMode::ReadOnly)) {
            qWarning() << "Error mapping frame, can not read the data.";
            return false;
        }

        process(local);

        local.unmap();
    }

    return true;
}

QImage VideoSurface::getImage()
{
    std::lock_guard<std::mutex> lock(mutex);
    return image.copy();
}

QPixmap VideoSurface::getPixmap()
{
    std::lock_guard<std::mutex> lock(mutex);
    return QPixmap::fromImage(image);
}

void VideoSurface::process(const QVideoFrame &frame)
{
    qDebug() << "Frame data:" << frame.mappedBytes() << static_cast<int>(frame.bits()[0]);

    QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());

    if (imageFormat != QImage::Format_Invalid) {
        QImage temp = QImage(frame.bits(), frame.width(), frame.height(), imageFormat);
        std::lock_guard<std::mutex> lock(mutex);
        image = temp.copy(); // Create deep copy
    } else {
        std::lock_guard<std::mutex> lock(mutex);
        image = QImage::fromData(frame.bits(), frame.mappedBytes());
    }

    if (!image.isNull()) {
        emit newImageReady();
    }
}
