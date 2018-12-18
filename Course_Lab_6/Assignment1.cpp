//
// Created by heze on 18-12-17.
// 帧差法实现运动物体检测
//

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void DetectFromCamera();            //摄像头运动物体检测
void DetectFromVideo(string src);   //视频运动物体检测
Mat MoveDetect(Mat, Mat);           //运动物体检测实现

int main() {
//    DetectFromCamera();
    DetectFromVideo("/home/heze/下载/videoTest.avi");
    return 0;
}


Mat MoveDetect(Mat temp, Mat frame) {
    Mat result = frame.clone();
    //背景和frame转灰度图
    Mat gray1, gray2;
    cvtColor(temp, gray1, COLOR_BGR2GRAY);
    cvtColor(frame, gray2, COLOR_BGR2GRAY);

    //高斯模糊
    GaussianBlur(gray1, gray1, Size(5, 5), 3);
    GaussianBlur(gray2, gray2, Size(5, 5), 3);

    //将背景和frame做差
    Mat diff;
    absdiff(gray1, gray2, diff);

    //阈值处理
    Mat diff_thresh;
    threshold(diff, diff_thresh, 15, 255, THRESH_BINARY);

    //腐蚀
    Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(5, 5));
    erode(diff_thresh, diff_thresh, kernel_erode);

    //膨胀
    Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(18, 18));
    dilate(diff_thresh, diff_thresh, kernel_dilate);

    //轮廓查找并绘制
    vector<vector<Point>> contours;
    findContours(diff_thresh, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
//    drawContours(result, contours, -1, Scalar(0, 0, 255), 2);

    //查找外界矩阵
    vector<Rect> boundRect(contours.size());
    for (int i = 0; i < contours.size(); i++) {
        boundRect[i] = boundingRect(contours[i]);
        rectangle(result, boundRect[i], Scalar(0, 255, 0), 2);
    }
    return result;
}

void DetectFromCamera() {
    //调用摄像头
    VideoCapture video(0);
    if (!video.isOpened()) {
        cout << "摄像头打开出错！" << endl;
        return;
    }
    int frameCount = video.get(CAP_PROP_FRAME_COUNT);   //获取视频帧数
    double FPS = video.get(CAP_PROP_FPS);
    Mat frame, temp, result;
    for (int i = 0; 1; i++) {
        video >> frame;
        if (frame.empty()) {
            cout << "视频出错！" << endl;
            break;
        }
        //设置第一帧
        if (i == 0)
            temp = frame;
        //帧差法运动检测
        result = MoveDetect(temp, frame);
        imshow("运动检测", result);
        if (waitKey(1) == 27)
            break;
        //设置帧差为3帧
        if (i % 3 == 0)
            temp = frame.clone();
    }
}

void DetectFromVideo(string src) {
    //打开视频
    VideoCapture video(src);
    if (!video.isOpened()) {
        cout << "视频打开出错！" << endl;
        return;
    }
    int frameCount = video.get(CAP_PROP_FRAME_COUNT);   //获取视频帧数
    double FPS = video.get(CAP_PROP_FPS);
    Mat frame, temp, result;
    for (int i = 0; i < frameCount; i++) {
        video >> frame;
        if (frame.empty()) {
            cout << "视频出错！" << endl;
            break;
        }
        //设置第一帧
        if (i == 0)
            temp = frame;
        //帧差法运动检测
        result = MoveDetect(temp, frame);
        imshow("运动检测", result);
        if (waitKey(1000.0 / FPS) == 27)
            break;
        //设置帧差为3帧
        if (i % 3 == 0)
            temp = frame.clone();
    }
}
