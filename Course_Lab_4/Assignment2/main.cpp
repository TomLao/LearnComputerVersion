//
// Created by heze on 18-12-4.
// 使用分水岭算法实现分割
//

#include <iostream>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

static Vec3b RandomColor(int value){
    value = value % 255;
    RNG rng;
    return Vec3b(rng.uniform(0, value), rng.uniform(0, value),rng.uniform(0, value));
}

int main(){
    Mat src = imread("Course_Lab_4/Resource/Lenna.png");
    Mat imageGray, imageContours, marksShows, afterWatershed, perspectiveImage, final;

    //灰度化，高斯滤波，Canny边缘检测
    cvtColor(src, imageGray, CV_RGB2GRAY);
    GaussianBlur(imageGray, imageGray, Size(5, 5), 2);
    Canny(imageGray, imageGray, 90, 150);
//    imshow("Canny", imageGray);

    //查找轮廓，将轮廓信息编号输入分水岭算法函数，标记注水点
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(imageGray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
    imageContours = Mat::zeros(imageGray.size(), CV_8UC1);
    Mat marks(imageGray.size(), CV_32S);
    marks = Scalar::all(0);
    int index = 0;
    int count = 0;
    while(index >= 0){
        //标记marks，对不同区域的轮廓编号，注水点。不同的灰度值不同
        drawContours(marks, contours, index, Scalar::all(count+1), 1, 8, hierarchy);
        drawContours(imageContours, contours, index, Scalar(255), 1, 8, hierarchy);
        index = hierarchy[index][0];
        count++;
    }
    convertScaleAbs(marks, marksShows);
//    imshow("marksShow", marksShows);
//    imshow("轮廓", imageContours);

    //分水岭运算
    watershed(src, marks);
    convertScaleAbs(marks, afterWatershed);
//    imshow("watershed", afterWatershed);


    //绘制分割区域，填充颜色信息
    perspectiveImage = Mat::zeros(src.size(), CV_8UC3);
    for(int i=0; i<marks.rows; i++){
        for(int j = 0; j<marks.cols; j++){
            index = marks.at<int>(i, j);
            if(marks.at<int>(i, j) == -1){
                perspectiveImage.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
            }else{
                perspectiveImage.at<Vec3b>(i, j) = RandomColor(index);
            }
        }
    }
    imshow("AfterColorFill", perspectiveImage);

    //与原图融合
    Mat wshed;
    addWeighted(src, 0.5, perspectiveImage, 0.5, 0, wshed);
    imshow("merge", wshed);


    waitKey(0);
    return 0;
}
