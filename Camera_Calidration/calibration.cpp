//
// Created by heze on 18-11-7.
// 使用张正友方法进行相机标定
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main() {
    ifstream fin("Camera_Calidration/test_img_list.txt");           //测试图片来源
    ofstream fout("Camera_Calidration/test_calibration_output.txt");//输出图片参数

    //***************************************************
    //step 1:读取图片并提取角点，对角点进行亚像素精确化
    //***************************************************
    int img_count = 0;//图片计数
    Size img_size;
    Size board_size = Size(9, 6);               //方格中间线有9列，6行，构成54个角点
    vector<Point2f> img_points_buf;             //一副图像的角点
    vector<vector<Point2f>> img_points_seq;     //所有图像的角点
    string filename;
    int count = -1;     //角点数

    while (getline(fin, filename)) {
        img_count++;
        cout << "img_count = " << img_count << endl;
        cout << "count = " << count;
        Mat img_input = imread(filename);
        if (img_count == 1) {
            img_size.width = img_input.cols;    //初始化图像宽和高
            img_size.width = img_input.rows;
        }

        //提取角点，先粗提取
        if (findChessboardCorners(img_input, board_size, img_points_buf) == 0) {
            cout << "Can not find corners!" << endl;
            exit(1);
        } else {
            Mat img_gray;
            cvtColor(img_input, img_gray, CV_RGB2GRAY);

            //亚像素精确化
            find4QuadCornerSubpix(img_gray, img_points_buf, Size(5, 5));//对粗提取的角点精确化
            img_points_seq.emplace_back(img_points_buf);                //保存亚像素角点

            //圈出角点并显示
            drawChessboardCorners(img_gray, board_size, img_points_buf, true);  //在图片中标记角点
            imshow("Camera Calibration", img_gray);
            waitKey(1000);
        }
    }

    int total = img_points_seq.size();  //已处理图片数
    cout << "Processed image total = " << total << endl;
    int cornerNum = board_size.width * board_size.height;   //计算角点数
    for (int i = 0; i < total; i++) {

        //控制台显示
        if (i % cornerNum == 0) {
            int count = -1;
            count = i / cornerNum;
            cout << "第：" << count + 1 << "图片的数据：" << endl;
        }

        if (i % 3 == 0) {
            cout << endl;
        } else {
            cout.width(10);
        }

        //输出所有角点
        cout << "-->" << img_points_seq[i][0].x;
        cout << "-->" << img_points_seq[i][0].y;
    }
    cout << "角点提取完成！" << endl;

    //*************************************************
    //step 2:相机标定计算
    //*************************************************

}