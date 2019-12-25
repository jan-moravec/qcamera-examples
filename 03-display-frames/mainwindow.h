#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraImageCapture>
#include <QActionGroup>

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

    QPointer<QCamera> camera;
    QPointer<QCameraImageCapture> capture;
    QPointer<VideoSurface> surface;

    QPointer<QActionGroup> ActionGroupCameras;
    QPointer<QActionGroup> ActionGroupFormats;
    QPointer<QActionGroup> ActionGroupResolutions;
};

#endif // MAINWINDOW_H
