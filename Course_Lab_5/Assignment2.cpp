//
// Created by heze on 18-12-9.
// 对脸部进行检测并保存，作为脸部识别训练样本。
// 就是拍照用的，识别在另一个FaceRecognize.cpp

#include <iostream>
#include <string>
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

string face_cascade = "/home/heze/下载/opencv-master/data/haarcascades/haarcascade_frontalface_default.xml";

int main(int argc, char **argv) {
    Mat frame, gray, roi, image1;
    CascadeClassifier a;             //创建脸部对象

    if (!a.load(face_cascade))     //如果读取文件不出错，则检测人脸
    {
        cout << "无法加载xml文件" << endl;
        return 0;
    }
    vector<Rect> faces;
    int count = 0;

    VideoCapture capture(0);
    while (1) {
        capture >> frame;
        if (frame.empty())
            break;

        string filename = format("Course_Lab_5/images/xrw/%d.jpg", count);//面部图像存储位置，需要预先创建好目录
        cvtColor(frame, gray, CV_BGR2GRAY);//转换为灰度图像
        equalizeHist(gray, gray);//直方图均衡化
        a.detectMultiScale(gray, faces, 1.3, 5);//识别

        for (int i = 0; i < faces.size(); i++) {
            rectangle(frame, faces[i], Scalar(0, 0, 255), 2, 8); //画出脸部矩形
            roi = gray(Rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height));//获取面部的roi尺寸
        }

        resize(roi, roi, Size(450, 450));//面部图像尺寸为450×450
        imshow("frame", frame);//显式视频窗口

        //按q拍照
        char key = waitKey(1);
        if (key == 'q' || key == 'Q') {
            imwrite(filename, roi);//保存面部图像
            cout << "save image " << count << endl;
            count++;
        }

    }
    return 0;
}
