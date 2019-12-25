#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QImage>
#include <QPixmap>
#include <mutex>

class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    VideoSurface(QObject *parent = nullptr);
    ~VideoSurface() override;

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const override;
    bool present(const QVideoFrame &frame) override;

    QImage getImage();
    QPixmap getPixmap();

signals:
    void newImageReady();

private:
    unsigned counter = 0;
    QImage image;
    std::mutex mutex;
    void process(const QVideoFrame &frame);
};

#endif // VIDEOSURFACE_H
