//
// Created by heze on 18-12-9.
// 脸部识别，使用截图处理好的人脸。
// 算法原理参考：https://docs.opencv.org/3.4.3/da/d60/tutorial_face_main.html
//
#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include "opencv2/face.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace std;
using namespace cv;
using namespace cv::face;

int main() {
    string face_cascade_name = "/home/heze/下载/opencv-master/data/haarcascades/haarcascade_frontalface_default.xml";
    string FACE_FILE_PATH = "Course_Lab_5/images/"; //照片基本路径

    CascadeClassifier face_cascade;

    if (!face_cascade.load(face_cascade_name))     //如果读取文件不出错，则检测人脸
    {
        cout << "无法加载xml文件" << endl;
        return 0;
    }

    // 准备数据
    vector<Rect> faces_det;
    vector<Mat> faces;
    vector<int> labels;
    vector<String> names = {"myface", "zzh", "xrw"};    //训练样本名
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.5f, 1.5f, 0, 2, CV_AA);//设置显示的字体


    // 按指定名字读取训练样本图像
    for (size_t j = 0; j < names.size(); j++) {
        for (size_t i = 0; i < 50; i++) {//注意这里的数量，应与面部图像数一致
            string face_path = FACE_FILE_PATH + names[j] + "/" + to_string(i) + ".jpg";
            faces.push_back(imread(face_path, 0));
            labels.push_back(j);
        }
    }

    // 训练：创建人脸识别类，这里可以选择特征脸，fisher，以及LBPH这三种方法：
    // EigenFaceRecognizer, FisherFaceRecognizer, LBPHFaceRecognizer
    Ptr<LBPHFaceRecognizer> face_recog = LBPHFaceRecognizer::create();
    face_recog->train(faces, labels);
    int face_id = 1;


    Mat frame, gray, roi;
    VideoCapture capture(0);
    while (1) {
        capture >> frame;
        if (frame.empty()) break;

        cvtColor(frame, gray, COLOR_BGR2GRAY);
        face_cascade.detectMultiScale(gray, faces_det, 1.3, 5);     //进行当前摄像头获取对象的面部识别

        for (int i = 0; i < faces_det.size(); i++) {
            rectangle(frame, faces_det[i], Scalar(0, 0, 255), 2, 8);//画出脸部矩形
            roi = gray(Rect(faces_det[i].x, faces_det[i].y, faces_det[i].width, faces_det[i].height));
			equalizeHist(roi, roi);
            resize(roi, roi, Size(450, 450));   //统一resize为450*450

            face_id = face_recog->predict(roi);// 识别
//            cout << face_id << endl;
            Point org;
            org.x = faces_det[i].x + 10;
            org.y = faces_det[i].y - 10;
            putText(frame, names[face_id], org, 2, 1, Scalar(0, 255, 0), 1, 8);//显示识别结果
        }
        imshow("查看脸部识别", frame);
        char ch = waitKey(1);
        if (ch == 'q' || ch == 'Q') {
            break;
        }
    }

    return 0;
}
