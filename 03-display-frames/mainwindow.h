#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraImageCapture>
#include <QActionGroup>
#include <memory>

#include "videosurface.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void refreshCameras();
    void refreshFormats();
    void refreshResolutions();

    void setCamera(QCameraInfo info);
    void setFormat(QString format);
    void setResolution(QSize resolution);

    void disconnectCamera();

    void updateImage();

private:
    Ui::MainWindow *ui;

    QList<QCameraInfo> cameras;
    QStringList formats;
    QList<QSize> resolutions;

    std::unique_ptr<QCamera> camera;
    std::unique_ptr<QCameraImageCapture> capture;
    std::unique_ptr<VideoSurface> surface;

    std::unique_ptr<QActionGroup> ActionGroupCameras;
    std::unique_ptr<QActionGroup> ActionGroupFormats;
    std::unique_ptr<QActionGroup> ActionGroupResolutions;
};

#endif // MAINWINDOW_H
