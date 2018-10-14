#include "opencvworker.h"
#include <opencv2/imgproc/imgproc.hpp>

OpenCvWorker::OpenCvWorker(QObject *parent) :
    QObject(parent),
    status(false),
    toggleStream(false),
    binaryThresholdEnable(false),
    binaryThreshold(127)
{
    cap = new cv::VideoCapture();
}

OpenCvWorker::~OpenCvWorker()
{
    if(cap->isOpened()) cap->release();
    delete cap;
}

void OpenCvWorker::checkIfDeviceAlreadyOpened(const int device)
{
    if(cap->isOpened()) cap->release();
    cap->open(device);
}

void OpenCvWorker::process()
{
    cv::cvtColor(_frameOriginal, _frameProcessed, cv::COLOR_BGR2GRAY);

    if(binaryThresholdEnable) {
        cv::threshold(_frameProcessed, _frameProcessed, binaryThreshold, 255, cv::THRESH_BINARY);
    }
}

void OpenCvWorker::receiveGrabFrame()
{
    if(!toggleStream) return;

    (*cap) >> _frameOriginal;
    if(_frameOriginal.empty()) return;

    process();

    QImage output((const unsigned char *)_frameProcessed.data, _frameProcessed.cols, _frameProcessed.rows, QImage::Format_Indexed8);
    emit sendFrame(output);
}

void OpenCvWorker::receiveSetup(const int device)
{
    checkIfDeviceAlreadyOpened(device);
    if(!cap->isOpened()) {
        status = false;
        return;
    }

    status = true;
}

void OpenCvWorker::receiveToggleStream() {
    toggleStream = !toggleStream;
}

void OpenCvWorker::receiveEnableBinaryThreshold() {
    binaryThresholdEnable = !binaryThresholdEnable;
}

void OpenCvWorker::receiveBinaryThreshold(int threshold) {
    binaryThreshold = threshold;
}




