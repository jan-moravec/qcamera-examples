#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include <QAbstractVideoSurface>

class VideoSurface : public QAbstractVideoSurface
{
public:
    VideoSurface();
    ~VideoSurface() override;

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const override;
    bool present(const QVideoFrame &frame) override;

private:
    void process(const QVideoFrame &frame);
};

#endif // VIDEOSURFACE_H
