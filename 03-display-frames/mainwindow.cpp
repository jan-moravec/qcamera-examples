#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    surface = new VideoSurface(this);
    connect(surface, &VideoSurface::newImageReady, this, &MainWindow::updateImage);

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

    delete ActionGroupCameras;
    ActionGroupCameras = new QActionGroup(this);
    ui->menuCameras->clear();

    if (cameras.size()) {
        ui->menuCameras->setEnabled(true);

        for (const QCameraInfo &info: cameras) {
            QAction *action = new QAction(info.description(), ActionGroupCameras); // capture->imageCodecDescription(codec)
            action->setCheckable(true);
            ui->menuCameras->addAction(action);
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
    delete ActionGroupFormats;
    ActionGroupFormats = new QActionGroup(this);
    ui->menuFormats->clear();

    if (capture) {
        formats = capture->supportedImageCodecs();
        ui->menuFormats->setEnabled(true);

        for (const QString &codec : formats) {
            QAction *action = new QAction(codec, ActionGroupFormats); // capture->imageCodecDescription(codec)
            action->setCheckable(true);
            ui->menuFormats->addAction(action);
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
    delete ActionGroupResolutions;
    ActionGroupResolutions = new QActionGroup(this);
    ui->menuResolutions->clear();

    if (capture) {
        resolutions = capture->supportedResolutions();
        ui->menuResolutions->setEnabled(true);

        for (const QSize &resolution : resolutions) {
            QAction *action = new QAction(QString::number(resolution.width()) + "x" + QString::number(resolution.height()), ActionGroupResolutions); // capture->imageCodecDescription(codec)
            action->setCheckable(true);
            ui->menuResolutions->addAction(action);
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
    delete camera;
    delete capture;
    camera = new QCamera(info, this);
    capture = new QCameraImageCapture(camera, this);

    camera->setViewfinder(surface);
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
    delete camera;
    delete capture;

    for (QAction *action : ui->menuCameras->actions()) {
        action->setChecked(false);
    }
    refreshFormats();
    refreshResolutions();
    ui->actionDisconnect->setEnabled(false);
}

void MainWindow::updateImage()
{
    ui->imageLabel->setPixmap(surface->getPixmap().scaledToWidth(ui->imageLabel->width(), Qt::SmoothTransformation));
}
