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

void VideoSurface::process(const QVideoFrame &frame)
{
    qDebug() << "Frame data:" << frame.mappedBytes() << static_cast<int>(frame.bits()[0]);
}
