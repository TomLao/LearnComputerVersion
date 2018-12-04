//
// Created by heze on 18-12-4.
// 使用opencv中findContours()函数返回图像的轮廓树，相关原理查看https://blog.csdn.net/guduruyu/article/details/69220296
// 设置轮廓检索模式为RETR_TREE返回四元组Vec4i，放入hierachy中，
// 其中[0]同级下一个轮廓，[1]同级上一个 ，[2]下一层级轮廓，[3]父轮廓上一级
//

#include <iostream>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
using namespace std;
using namespace cv;

int main() {

    char path[100];
    for(int i=1; i<=5; i++) {
        sprintf(path, "Course_Lab_4/Resource/sample%d.png", i);

//    Mat src = imread("Course_Lab_4/Resource/sample.png");
        Mat src = imread(path);
        if (src.empty()) {
            cout << "Fail to load image!";
            exit(0);
        }

        Mat imgGray, imgBinary;
        vector<vector<Point>> contours;
        vector<Vec4i> hierachy;     //保存轮廓树信息
        cvtColor(src, imgGray, CV_RGB2GRAY);
        threshold(imgGray, imgBinary, 0, 255, THRESH_BINARY | THRESH_OTSU); //otsu自适应二值化法

        //指定hierachy，放回轮廓树的信息，RETR_TREE表示提取所有轮廓并组织成轮廓嵌套的完整层级结构
        findContours(imgBinary, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE);

        RNG rng(12345);
        int index = -1;
        for (size_t i = 1; i < contours.size(); i++) {  //对每一个轮廓判断
            if (contourArea(contours[i]) < 5000 && contourArea(contours[i]) > 1000) {   //限制轮廓面积，需要先验估计。。。
//            if (hierachy[i][0] == -1) {
                if (hierachy[hierachy[i][2]][3] == i && hierachy[hierachy[hierachy[i][2]][0]][3] == i) {
                    //表示i有两个子节点，因为二维码内部只有两个轮廓，琢磨琢磨，可能有点绕
                    Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
                    drawContours(src, contours, i, color, 2, 8);
                }
//            }
            }
        }
        imshow("sample", src);
        waitKey(0);
    }
}