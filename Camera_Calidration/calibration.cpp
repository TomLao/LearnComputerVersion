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
            img_size.height = img_input.rows;
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
//            waitKey(1000);
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
    Size square_size = Size(10, 10);        //棋盘格大小
    vector<vector<Point3f>> object_points;  //保存棋盘格上角点的三维坐标

    //相机内外参
    Mat camera_matrix = Mat(3, 3, CV_32FC1, Scalar::all(0));    //相机内参矩阵
    vector<int> point_counts;   //图像角点数量
    Mat dist_coeffs = Mat(1, 5, CV_32FC1, Scalar::all(0));        //相机5个几遍函数：k1,k2,p1,p2,p3
    vector<Mat> tvecs_mat;  //图像的旋转向量
    vector<Mat> rvecs_mat;  //图像的平移向量
    //初始化标定板角点的三维坐标
    int i, j, t;
    for (t = 0; t < img_count; t++) {
        vector<Point3f> temp_point_vec;
        for (i = 0; i < board_size.height; i++) {
            for (j = 0; j < board_size.width; j++) {
                Point3f real_point;
                //假设标定板放在世界坐标系中z=0的平面上
                real_point.x = i * square_size.width;
                real_point.y = j * square_size.height;
                real_point.z = 0;
                temp_point_vec.push_back(real_point);
            }
        }
        object_points.emplace_back(temp_point_vec);
    }
    //初始化每幅图像中的角点数量，假定每幅图像中都可以看到完整的标定板
    for (i = 0; i < img_count; i++) {
        point_counts.push_back(board_size.width * board_size.height);
    }

    //开始标定
    calibrateCamera(object_points, img_points_seq, img_size, camera_matrix, dist_coeffs, rvecs_mat, tvecs_mat, 0);
    cout << "开始评价标定结果" << endl;
    double totalErr = 0.0;      //所有图像的平均误差的总和
    double err = 0.0;           //每幅图像的平均误差
    vector<Point2f> imgPoints2; //保存重新计算得到的投影点
    cout << "每幅图像的标定误差：" << endl;
    fout << "每幅图像的标定误差：" << endl;
    for (i = 0; i < img_count; i++) {
        vector<Point3f> tempPointVec = object_points[i];
        //通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点
        projectPoints(tempPointVec, rvecs_mat[i], tvecs_mat[i], camera_matrix, dist_coeffs, imgPoints2);
        //计算新的投影点和旧的投影点之间的误差
        vector<Point2f> tempImagePoint = img_points_seq[i];
        Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
        Mat imagePoints2Mat = Mat(1, imgPoints2.size(), CV_32FC2);
        for (int j = 0; j < tempImagePoint.size(); j++) {
            imagePoints2Mat.at<Vec2f>(0, j) = Vec2f(imgPoints2[j].x, imgPoints2[j].y);
            tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
        }
        err = norm(imagePoints2Mat, tempImagePointMat, NORM_L2);
        totalErr += err /= point_counts[i];
        std::cout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << endl;
        fout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << endl;
    }
    std::cout << "总体平均误差：" << totalErr / img_count << "像素" << endl;
    fout << "总体平均误差：" << totalErr / img_count << "像素" << endl;
    std::cout << "评价完成！" << endl;

    //保存标定结果
    std::cout << "开始保存标定结果。。。" << endl;
    Mat rotationMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0));   //保存每幅图像的旋转矩阵
    fout << "相机内参数矩阵：" << endl;
    fout << camera_matrix << endl << endl;
    fout << "畸变系数" << endl;
    fout << dist_coeffs << endl << endl;
    for (int i = 0; i < img_count; i++) {
        fout << "第" << i + 1 << "幅图像的旋转向量：" << endl;
        fout << tvecs_mat[i] << endl;
        //旋转向量转换为旋转矩阵
        Rodrigues(tvecs_mat[i], rotationMatrix);
        fout << "第" << i + 1 << "幅图像的旋转矩阵：" << endl;
        fout << rotationMatrix << endl;
        fout << "第" << i + 1 << "幅图像的平移向量：" << endl;
        fout << rvecs_mat[i] << endl << endl;
    }
    std::cout << "完成保存" << endl;
    fout << endl;


}