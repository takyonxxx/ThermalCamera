#include "process_frame.h"


ProcessFrame::ProcessFrame(QObject* parent)
    :QObject(parent)
{
}

void ProcessFrame::processFrame(Mat &frameRGB, int constant, int type) {
    if(type == 1)
        processFrame1(frameRGB, constant);
    else if(type == 2)
        processFrame2(frameRGB, constant);
    else if(type == 3)
        processFrame3(frameRGB, constant);

}

void ProcessFrame::processFrame1(Mat &frameRGB, int constant) {

    // Convert the frame to HSV
    cv::cvtColor(frameRGB, frameRGB, cv::COLOR_BGR2HSV);

    // Define a range for green color in HSV
    cv::Scalar lowerGreen(constant, 30, 30);  // Use constant for hue
    cv::Scalar upperGreen(constant + 50, 255, 255);  // Adjust these values based on your specific green color

    // Create a binary mask where pixels within the green range are white, others are black
    cv::Mat greenMask;
    cv::inRange(frameRGB, lowerGreen, upperGreen, greenMask);

    // Create a matrix with the original frame values for non-green pixels
    cv::Mat nonGreenMatrix = frameRGB.clone();
    nonGreenMatrix.setTo(cv::Scalar(0, 0, 0), greenMask);

    // Set green pixels in the original frame to a specific green color
    frameRGB.setTo(cv::Scalar(constant, 255, 255), greenMask);

    // Blend the modified green frame with the original frame
    frameRGB = frameRGB + nonGreenMatrix;

    // Convert back to BGR
    cv::cvtColor(frameRGB, frameRGB, cv::COLOR_HSV2BGR);
}


void ProcessFrame::processFrame2(cv::Mat& thermalFrame, int constant) {


    // Apply a colormap to enhance visualization (adjust based on your camera's characteristics)
    cv::applyColorMap(thermalFrame, thermalFrame, cv::COLORMAP_JET);

    // Normalize the thermal data to the range [0, 255] for proper visualization
    cv::normalize(thermalFrame, thermalFrame, 0, 255, cv::NORM_MINMAX);

    // Threshold the image to highlight regions with temperatures above a certain threshold
    cv::Mat hotRegions;
    cv::threshold(thermalFrame, hotRegions, constant, 255, cv::THRESH_BINARY);

    // Create a matrix with a custom color for hot regions
    cv::Mat hotColorMatrix(thermalFrame.size(), thermalFrame.type(), cv::Scalar(0, 0, 255));

    // Blend the hot color matrix with the original thermal frame
    cv::bitwise_and(hotColorMatrix, hotRegions, hotColorMatrix);
    cv::addWeighted(thermalFrame, 1.0, hotColorMatrix, 0.5, 0.0, thermalFrame);
}

void ProcessFrame::processFrame3(cv::Mat& thermalFrame, int constant) {

    cv::Mat reversedColormap;
    cv::applyColorMap(thermalFrame, reversedColormap, cv::COLORMAP_JET);
    cv::bitwise_not(reversedColormap, reversedColormap);

    // Normalize the thermal data to the range [0, 255] for proper visualization
    cv::normalize(reversedColormap, reversedColormap, 0, 255, cv::NORM_MINMAX);

    // Threshold the image to highlight regions with temperatures above a certain threshold
    cv::Mat hotRegions;
    cv::threshold(reversedColormap, hotRegions, constant, 255, cv::THRESH_BINARY);

    // Create a matrix with a custom color for hot regions
    cv::Mat hotColorMatrix(reversedColormap.size(), reversedColormap.type(), cv::Scalar(255, 0, 0)); // Red for hot

    // Blend the hot color matrix with the original reversed colormap
    cv::bitwise_and(hotColorMatrix, hotRegions, hotColorMatrix);
    cv::addWeighted(reversedColormap, 1.0, hotColorMatrix, 0.5, 0.0, thermalFrame);
}


