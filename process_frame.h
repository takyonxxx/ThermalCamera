#ifndef RPPG_hpp
#define RPPG_hpp

#include <QDebug>
#include <QCamera>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class ProcessFrame : public QObject
{
    Q_OBJECT

public:
    explicit ProcessFrame(QObject *parent = nullptr);

    void processFrame(Mat &frameRGB, int, int);
    void processFrame1(Mat &frameRGB, int);
    void processFrame2(Mat &frameRGB, int);
    void processFrame3(Mat &frameRGB, int);

    int64_t get_current_time()
    {
        int64_t tickCount = cv::getTickCount();
        return static_cast<int64_t>((tickCount * 1000.0) / cv::getTickFrequency());
    }

private:   
    QString info{};

signals:
    void sendInfo(QString);
};


#endif /* RPPG_hpp */
