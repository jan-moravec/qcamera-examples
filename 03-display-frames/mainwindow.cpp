#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    surface = std::make_unique<VideoSurface>();
    connect(surface.get(), &VideoSurface::newImageReady, this, &MainWindow::updateImage);

    connect(ui->actionRefresh, &QAction::triggered, this, &MainWindow::refreshCameras);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::disconnectCamera);

    refreshCameras();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshCameras()
{
    disconnectCamera();
    cameras = QCameraInfo::availableCameras();

    ActionGroupCameras = std::make_unique<QActionGroup>(this);
    ui->menuCameras->clear();

    if (cameras.size()) {
        ui->menuCameras->setEnabled(true);

        for (const QCameraInfo &info: cameras) {
            QAction *action = ui->menuCameras->addAction(info.description());
            ActionGroupCameras->addAction(action);
            connect(action, &QAction::triggered, [=](){
                setCamera(info);
            });
        }

        for (const QCameraInfo &info: cameras) {
            qDebug() << "Opening camera:" << info.deviceName();
            qDebug() << "\t- Description:" << info.description();
            qDebug() << "\t- Position:" << info.position();
        }

    } else {
        ui->menuCameras->setDisabled(true);
    }

}

void MainWindow::refreshFormats()
{
    ActionGroupFormats = std::make_unique<QActionGroup>(this);
    ui->menuFormats->clear();

    if (capture) {
        formats = capture->supportedImageCodecs();
        ui->menuFormats->setEnabled(true);

        for (const QString &codec : formats) {
            QAction *action = ui->menuFormats->addAction(capture->imageCodecDescription(codec));
            ActionGroupFormats->addAction(action);
            connect(action, &QAction::triggered, [=](){
                setFormat(codec);
            });
        }

        qDebug() << "Image Codecs:";
        for (const QString &codec : formats) {
            qDebug() << "\t\t-" << codec << capture->imageCodecDescription(codec);
        }

    } else {
        ui->menuFormats->setDisabled(true);
    }
}

void MainWindow::refreshResolutions()
{
    ActionGroupResolutions = std::make_unique<QActionGroup>(this);
    ui->menuResolutions->clear();

    if (capture) {
        resolutions = capture->supportedResolutions();
        ui->menuResolutions->setEnabled(true);

        for (const QSize &resolution : resolutions) {
            QAction *action = ui->menuResolutions->addAction(QString::number(resolution.width()) + "x" + QString::number(resolution.height()));
            ActionGroupResolutions->addAction(action);
            connect(action, &QAction::triggered, [=](){
                setResolution(resolution);
            });
        }

        qDebug() << "Resolutions:";
        for (const QSize &resolution : resolutions) {
            qDebug() << "\t\t-" << resolution.width() << "x" << resolution.height();
        }

    } else {
        ui->menuResolutions->setDisabled(true);
    }
}

void MainWindow::setCamera(QCameraInfo info)
{
    camera = std::make_unique<QCamera>(info);
    capture = std::make_unique<QCameraImageCapture>(camera.get());

    camera->setViewfinder(surface.get());
    camera->load();
    camera->start();

    refreshFormats();
    refreshResolutions();

    ui->actionDisconnect->setEnabled(true);
}

void MainWindow::setFormat(QString format)
{
    if (capture) {
        QImageEncoderSettings settings = capture->encodingSettings();
        settings.setCodec(format);
        settings.setQuality(QMultimedia::EncodingQuality::VeryHighQuality);
        capture->setEncodingSettings(settings);
    }
}

void MainWindow::setResolution(QSize resolution)
{
    if (capture) {
        QImageEncoderSettings settings = capture->encodingSettings();
        settings.setResolution(resolution);
        capture->setEncodingSettings(settings);
    }
}

void MainWindow::disconnectCamera()
{
    camera.reset();
    capture.reset();

    refreshFormats();
    refreshResolutions();
    ui->actionDisconnect->setEnabled(false);
}

void MainWindow::updateImage()
{
    ui->imageLabel->setPixmap(surface->getPixmap().scaledToWidth(ui->imageLabel->width(), Qt::SmoothTransformation));
}
