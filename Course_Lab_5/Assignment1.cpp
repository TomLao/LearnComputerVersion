//
// Created by heze on 18-12-8.
// 利用Adaboost算法实现人脸的实时检测，测试算法的检测效率和准确性。
//

#include <iostream>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

int main() {
    VideoCapture inputVideo(0);
    Mat frame_src, frame_gray;
//    frame_src = imread("/run/media/heze/big/MyProject/LearnComputerVersion/Course_Lab_5/people.jpg");

    //xml文件去github下载，链接：https://github.com/opencv/opencv
    String face_cascade_name = "/home/heze/下载/opencv-master/data/haarcascades/haarcascade_frontalface_alt.xml";
    String eye_cascade_name = "/home/heze/下载/opencv-master/data/haarcascades/haarcascade_eye.xml";

    CascadeClassifier face_cascade;
    CascadeClassifier eye_cascade;
    if (!face_cascade.load(face_cascade_name)) {
        printf("can not load the haar_face_Cascader file \n");
        return -1;
    }
    if (!eye_cascade.load(eye_cascade_name)) {
        printf("can not load the haar_eye_Cascader file \n");
        return -1;
    }
    if (!inputVideo.isOpened()) {
        cout << "fall" << endl;
        exit(1);
    }

    while (1) {
        inputVideo >> frame_src;
        if (frame_src.empty())
            exit(0);
        resize(frame_src, frame_src, Size(frame_src.cols / 2, frame_src.rows / 2), 0, 0, INTER_LINEAR);
        cvtColor(frame_src, frame_gray, COLOR_BGR2GRAY);

        std::vector<Rect> faces;
        std::vector<Rect> eyes;
        face_cascade.detectMultiScale(frame_gray, faces, 1.3, 5);
        for (int i = 0; i < faces.size(); i++) {
            rectangle(frame_src, faces[i], Scalar(0, 0, 255), 2, 8, 0);
            Mat roiGray = frame_gray(faces[i]);
            Mat roiColor = frame_src(faces[i]);
            eye_cascade.detectMultiScale(roiGray, eyes);
            for (size_t j = 0; j < eyes.size(); j++) {
                rectangle(roiColor, eyes[j], Scalar(0, 255, 0), 2, 8, 0);
            }
        }
        imshow("result", frame_src);

        //按 q 或 Q 退出
        char key = waitKey(1);
        if (key == 'q' || key == 'Q') {
            break;
        }
    }

    return 0;
}

