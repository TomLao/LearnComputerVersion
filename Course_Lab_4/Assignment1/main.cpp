//
// Created by heze on 18-12-3.
//采用otsu自适应二值化方法，
//通过轮廓特征信息来寻找图像中二维码位置，
//其中计算了图形边缘点的consine值并筛选。
//
#include <iostream>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

double angle(Point p1, Point p2, Point p0) {
    double dx1 = p1.x - p0.x;
    double dy1 = p1.y - p0.y;
    double dx2 = p2.x - p0.x;
    double dy2 = p2.y - p2.y;
    return (dx1 * dx2 + dy1 + dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}

void findArea(Mat &image, vector<vector<Point>> &contours) {
    threshold(image, image, 0, 255, THRESH_BINARY | THRESH_OTSU);

    vector<vector<Point>> tempContours;
    vector<Point> approx;
    findContours(image, tempContours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    for (size_t i = 0; i < tempContours.size(); i++) {
        approxPolyDP(Mat(tempContours[i]), approx, arcLength(Mat(tempContours[i]), true) * 0.02, true);
        if (approx.size() == 4 && fabs(contourArea(Mat(approx))) > 1000 && isContourConvex(Mat(approx))) {
            double maxCosine = 0;
            for (int j = 2; j < 5; j++) {
                double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
                maxCosine = MAX(maxCosine, cosine);
            }
            if (maxCosine < 2) {
                contours.push_back(approx);
            }
        }
    }
}

void drawArea(Mat &image, vector<vector<Point>> &contours) {
    for (size_t i = 0; i < contours.size(); i++) {
        const Point *p = &contours[i][0];

        int n = (int) contours[i].size();
        //dont detect the border
        if (p->x > 10 && p->y > 10) {
            polylines(image, &p, &n, 1, true, Scalar(0, 255, 0), 3, LINE_AA);
            int center_x = ((contours[i][0].x + contours[i][1].x) / 2 + (contours[i][2].x + contours[i][3].x) / 2) / 2;
            int center_y = ((contours[i][1].y + contours[i][2].y) / 2 + (contours[i][0].y + contours[i][3].y) / 2) / 2;
            Point center(center_x, center_y);
//            putText(image, "+", center, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0));
            line(image, Point(center_x - 5, center_y), Point(center_x + 5, center_y), Scalar(0, 255, 0), 2);  //竖直线
            line(image, Point(center_x, center_y - 5), Point(center_x, center_y + 5), Scalar(0, 255, 0), 2);  //横直线
            cout << "中心点：x=" << center_x << ", y=" << center_y << endl;
            double degera = atan(
                    (double) (contours[i][3].y - contours[i][0].y) / (double) (contours[i][3].x - contours[i][0].x));
            cout << "角度为：" << degera << endl;
        }
    }

    imshow("draw", image);
}

int main() {
    Mat src = imread("Course_Lab_4/Resource/sample1.png");
    if (src.empty()) {
        cout << "Fail to load image!";
        exit(0);
    }

    Mat imgGray, imgBinary;
    vector<vector<Point>> contours;
    cvtColor(src, imgGray, CV_RGB2GRAY);

    findArea(imgGray, contours);
    drawArea(src, contours);

    waitKey(0);
    return 0;
}