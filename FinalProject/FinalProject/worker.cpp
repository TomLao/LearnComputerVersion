#include "mainwindow.h"

//主控函数
Mat start(Mat frame){

}

//转为hls颜色空间
Mat convertHls(Mat image){
    Mat converted;
    cvtColor(image, converted, CV_RGB2HLS);
    return converted;
}

//根据hls颜色空间选取白黄车道线
Mat selectWhiteYellow(Mat image){
    Mat converted = convertHls(image);
    Mat mask, maskWhite, maskYellow;
    //取白色和黄色mask
    inRange(converted, Scalar(0, 200, 0), Scalar(255, 255, 255), maskWhite);
    inRange(converted, Scalar(10, 0, 100), Scalar(40, 255, 255), maskYellow);
    //合并
    bitwise_or(maskWhite, maskYellow, mask);
    bitwise_and(image, image, mask);
    return image;
}

//灰度化
Mat convertGrayScale(Mat image){
    Mat converted;
    cvtColor(image, converted, CV_RGB2GRAY);
    return converted;
}

//高斯模糊
Mat smoothing(Mat image, int kernelSize){
    Mat smoothed;
    GaussianBlur(image, smoothed, Size(kernelSize, kernelSize), 0);
    return smoothed;
}

//canny边缘检测
Mat detectEdge(Mat image, int lowThreshold, int highThreshold){
    Mat edge;
    Canny(image, edge, lowThreshold, highThreshold);
    return edge;
}

//选取车道线，区域过滤，去除噪声
Mat selectRegion(Mat image){
    float rows = image.rows, cols = image.cols;
    Point rootPoints[1][4];
    rootPoints[0][0] = Point(cols*0.1, rows*0.95);
    rootPoints[0][1] = Point(cols*0.4, rows*0.6);
    rootPoints[0][2] = Point(cols*0.6, rows*0.6);
    rootPoints[0][3] = Point(cols*0.9, rows*0.95);
    const Point* ppt[1]={rootPoints[0]};
    Mat mask = image.clone();
    int npt[] = { 255 };
    fillPoly(mask, ppt, npt, 1, Scalar(255, 255, 255));
    return mask;
}

//Hough线变换
Mat houghLine(Mat image){
    Mat lines;
    const double PI = std::atan(1.0)*4;
    HoughLinesP(image, lines, 1, PI/180, 20, 20, 300);
    return lines;
}

//在原图上画线
