#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>
#include <QFile>
#include <QStandardPaths>
#include <QGraphicsPixmapItem>
#include <QMediaDevices>
#include "frames.h"
#include "process_frame.h"

#if defined(Q_OS_ANDROID)
#include <QJniObject>
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace cv;
Q_DECLARE_METATYPE(cv::Mat);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* instance() { return m_instance; }
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUI();
    void setupCamera();

#if defined(Q_OS_ANDROID)
    void requestAndroidPermissions();
#endif

    QGraphicsPixmapItem pixmap;
    QMediaCaptureSession m_captureSession;
    QScopedPointer<QCamera> m_camera;
    Frames *m_frames{nullptr};
    ProcessFrame *process_frame{nullptr};
    int constantValue = 0;
    int processType = 1;
    Ui::MainWindow *ui;
    static inline MainWindow* m_instance = nullptr;

signals:
    void cameraPermissionGranted();

private slots:
    void processFrame(QVideoFrame&);
    void processImage(QImage&);
    void printInfo(QString);
    void onCameraListUpdated(const QStringList &);
    void on_pushExit_clicked();
    void on_cameraComboBox_currentIndexChanged(int index);
#if defined(Q_OS_ANDROID)
    void onCameraPermissionGranted();
#endif
    void on_constantHorizontalSlider_valueChanged(int value);
    void on_typeComboBox_currentIndexChanged(int index);
};

#endif // MAINWINDOW_H
