#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Thermal Camera");
    setStyleSheet("background-color: #1E1E2E;");

    setupUI();
#if defined(Q_OS_ANDROID)
    connect(this, &MainWindow::cameraPermissionGranted, this, &MainWindow::onCameraPermissionGranted);
#endif
    process_frame = new ProcessFrame();
    connect(process_frame, &ProcessFrame::sendInfo, this, &MainWindow::printInfo);

#if defined(Q_OS_ANDROID)
    requestAndroidPermissions();
#else
    setupCamera();
#endif
}

void MainWindow::setupUI()
{
    // Define color palette
    const QString PRIMARY_BG = "#1E1E2E";      // Dark background
    const QString SECONDARY_BG = "#2D2D44";    // Slightly lighter background
    const QString ACCENT_COLOR = "#6E6E9F";    // Purple accent
    const QString TEXT_COLOR = "#FFFFFF";      // Pure white text
    const QString EXIT_BTN_COLOR = "#FF4B4B";  // Red exit button

    const QString SLIDER_BG = "#363654";        // Darker purple for slider background
    const QString SLIDER_ACTIVE = "#8686BF";    // Brighter purple for active part
    const QString SLIDER_HANDLE = "#FFFFFF";    // White handle
    const QString SLIDER_HANDLE_HOVER = "#E0E0E0"; // Slightly darker white for hover

    const QString LABEL_BG = "#363654";      // Same as slider background
    const QString LABEL_TEXT = "#FFFFFF";     // White text

    // Graphics view with subtle shadow effect
    ui->graphicsView->setStyleSheet(QString(
                                        "QGraphicsView {"
                                        "   background-color: %1;"
                                        "   border: 2px solid %2;"
                                        "   border-radius: 15px;"
                                        "   margin: 8px;"
                                        "}"
                                        "QGraphicsView:focus {"
                                        "   border-color: %3;"
                                        "}"
                                        ).arg(SECONDARY_BG, ACCENT_COLOR, TEXT_COLOR));

    QString comboBoxStyle = QString(
        "QComboBox {"
        "   font-size: 16pt;"
        "   color: white;"
        "   background-color: %1;"
        "   border-radius: 8px;"
        "   padding: 8px 12px;"
        "   min-height: 30px;"
        "}"
        "QComboBox::drop-down {"
        "   border: none;"
        "   width: 30px;"
        "}"
        "QComboBox::down-arrow {"
        "   image: url(:/icons/dropdown_arrow.png);"
        "   width: 12px;"
        "   height: 12px;"
        "}"
        "QComboBox QAbstractItemView {"
        "   color: white;"
        "   background-color: %1;"
        "   selection-color: white;"
        "   selection-background-color: %2;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 4px;"
        "}"
        "QComboBox QAbstractItemView::item {"
        "   min-height: 30px;"
        "   padding: 4px;"
        "}"
        "QComboBox QAbstractItemView::item:hover {"
        "   background-color: %2;"
        "}"
                                ).arg(ACCENT_COLOR, PRIMARY_BG);

    ui->cameraComboBox->setStyleSheet(comboBoxStyle);
    ui->typeComboBox->setStyleSheet(comboBoxStyle);

    // Exit button with hover effect
    ui->pushExit->setStyleSheet(QString(
                                    "QPushButton {"
                                    "   font-size: 18pt;"
                                    "   font-weight: bold;"
                                    "   color: white;"
                                    "   background-color: %1;"
                                    "   border-radius: 8px;"
                                    "   padding: 10px 20px;"
                                    "   margin: 5px;"
                                    "}"
                                    "QPushButton:hover {"
                                    "   background-color: #FF6B6B;"
                                    "}"
                                    "QPushButton:pressed {"
                                    "   background-color: #FF2D2D;"
                                    "}"
                                    ).arg(EXIT_BTN_COLOR));

    ui->constantHorizontalSlider->setStyleSheet(QString(
                                                    "QSlider {"
                                                    "   min-height: 30px;"
                                                    "   max-height: 30px;"
                                                    "   margin: 8px;"
                                                    "   background: transparent;"  // Make main background transparent
                                                    "}"
                                                    "QSlider::groove:horizontal {"
                                                    "   height: 12px;"            // Increased height
                                                    "   background: %1;"
                                                    "   border-radius: 6px;"      // Increased radius
                                                    "   border: 1px solid #4A4A6A;" // Added border for depth
                                                    "}"
                                                    "QSlider::handle:horizontal {"
                                                    "   background: %2;"
                                                    "   border: 2px solid %1;"    // Added border
                                                    "   width: 20px;"             // Slightly larger handle
                                                    "   height: 20px;"
                                                    "   margin: -5px 0;"
                                                    "   border-radius: 10px;"
                                                    "}"
                                                    "QSlider::handle:horizontal:hover {"
                                                    "   background: %3;"
                                                    "   border: 2px solid %1;"
                                                    "}"
                                                    "QSlider::sub-page:horizontal {"
                                                    "   background: %4;"          // Active (filled) part
                                                    "   border-radius: 6px;"
                                                    "}"
                                                    "QSlider::add-page:horizontal {"
                                                    "   background: %1;"          // Inactive part
                                                    "   border-radius: 6px;"
                                                    "}"
                                                    ).arg(SLIDER_BG, SLIDER_HANDLE, SLIDER_HANDLE_HOVER, SLIDER_ACTIVE));

    ui->labelConstant->setStyleSheet(QString(
                                         "QLabel {"
                                         "   color: %1;"
                                         "   background-color: %2;"
                                         "   font-size: 14pt;"
                                         "   font-weight: bold;"
                                         "   padding: 4px 12px;"
                                         "   border-radius: 6px;"
                                         "   min-width: 40px;"         // Ensure consistent width
                                         "   text-align: center;"
                                         "   margin: 0px 8px;"         // Add some spacing from slider
                                         "}"
                                         ).arg(LABEL_TEXT, LABEL_BG));

    // When setting the value, you might want to use fixed precision
    ui->labelConstant->setText(QString::number(0, 'f', 0));  // Shows one decimal place

    // To ensure the label is aligned properly with the slider, you might want to set its alignment
    ui->labelConstant->setAlignment(Qt::AlignCenter);

    // Disable scrollbars
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Setup graphics scene
    ui->graphicsView->setScene(new QGraphicsScene(this));

    // Set window to fill available screen space
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QRect screenGeometry = primaryScreen->availableGeometry();
    setGeometry(screenGeometry);

    // Add pixmap to scene
    ui->graphicsView->scene()->addItem(&pixmap);

    // Initialize controls
    ui->constantHorizontalSlider->setValue(0);
    ui->typeComboBox->setCurrentIndex(1);
}

void MainWindow::setupCamera()
{
    if (!m_frames) {
        m_frames = new Frames();
        connect(m_frames, &Frames::sendInfo, this, &MainWindow::printInfo);
        connect(m_frames, &Frames::frameCaptured, this, &MainWindow::processFrame);
        connect(m_frames, &Frames::cameraListUpdated, this, &MainWindow::onCameraListUpdated);
        m_frames->initializeCameraDevices();
    }
}

#if defined(Q_OS_ANDROID)
void MainWindow::requestAndroidPermissions()
{
    QJniObject activity = QJniObject::callStaticObjectMethod(
        "org/qtproject/qt/android/QtNative",
        "activity",
        "()Landroid/app/Activity;"
        );

    if (!activity.isValid())
        return;

    jint permission = activity.callMethod<jint>(
        "checkSelfPermission",
        "(Ljava/lang/String;)I",
        QJniObject::fromString("android.permission.CAMERA").object()
        );

    if (permission != 0) { // PERMISSION_GRANTED = 0
        QJniObject::callStaticMethod<void>(
            "org/tbiliyor/thermal/camera/MainActivity",
            "requestPermission",
            "(Ljava/lang/String;)V",
            QJniObject::fromString("android.permission.CAMERA").object()
            );
    } else {
        setupCamera(); // Call setupCamera directly if permission already granted
    }
}

void MainWindow::onCameraPermissionGranted()
{
    setupCamera(); // Remove timer delay, call setupCamera directly
}
#endif

void MainWindow::processFrame(QVideoFrame &frame)
{
    if (frame.isValid())
    {
        QVideoFrame cloneFrame(frame);
        cloneFrame.map(QVideoFrame::ReadOnly);

        QImage img = cloneFrame.toImage();
        cloneFrame.unmap();

        img = img.convertToFormat(QImage::Format_RGB888);
        Mat frameRGB(img.height(),
                     img.width(),
                     CV_8UC3,
                     img.bits(),
                     img.bytesPerLine());

        if(!frameRGB.empty())
        {
            process_frame->processFrame(frameRGB, constantValue, processType);
            QImage img_face((uchar*)frameRGB.data, frameRGB.cols, frameRGB.rows, frameRGB.step, QImage::Format_RGB888);
            pixmap.setPixmap( QPixmap::fromImage(img_face));
            ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatioByExpanding);
        }
    }
}

void MainWindow::processImage(QImage &img_face)
{
    pixmap.setPixmap(QPixmap::fromImage(img_face));
    ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
}

void MainWindow::printInfo(QString info)
{
    qDebug() << info;
}

void MainWindow::on_pushExit_clicked()
{
    qApp->exit();
}

void MainWindow::onCameraListUpdated(const QStringList &cameraDevices)
{
    ui->cameraComboBox->clear();
    ui->cameraComboBox->addItems(cameraDevices);
}

void MainWindow::on_cameraComboBox_currentIndexChanged(int index)
{
    QString selectedText = ui->cameraComboBox->itemText(index);
    m_frames->setCamera(selectedText);
}

MainWindow::~MainWindow()
{
    if(m_frames)
        delete m_frames;

    if(process_frame)
        delete process_frame;

    delete ui;
}

#if defined(Q_OS_ANDROID)
extern "C" {
JNIEXPORT void JNICALL
Java_org_tbiliyor_thermal_camera_MainActivity_notifyCameraPermissionGranted(JNIEnv *env, jobject obj)
{
    if (MainWindow::instance()) {
        QMetaObject::invokeMethod(MainWindow::instance(), "cameraPermissionGranted");
    }
}
}
#endif

void MainWindow::on_constantHorizontalSlider_valueChanged(int value)
{
    constantValue = value;
    ui->labelConstant->setText(QString::number(value));
}


void MainWindow::on_typeComboBox_currentIndexChanged(int index)
{
    processType = index + 1;
}

