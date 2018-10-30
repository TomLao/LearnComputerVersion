//#include "mainwindow.h"
//#include <QApplication>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
//}

#include<opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;

struct points4img{
    vector<Point2f> points;
    Mat img;
};

void mouseHandler(int event, int x, int y, int flags, void *param){
    //左键按下事件
    if(EVENT_LBUTTONDOWN == event){
        points4img *data = (points4img*)param;
        circle(data->img, Point(x, y), 1, Scalar(0, 0, 255), 5, CV_AA);
        //每次点击都显示红点
        imshow("board", data->img);
        if(data->points.size() < 4){
            //加入点向量
            data->points.push_back(Point2f(x, y));
        }
    }
}

int main(){
    Mat lena = imread("D:\\MyProject\\LearnComputerVersion\\Course_Lab_2\\Assignment2\\lena.jpg");
    Mat board = imread("D:\\MyProject\\LearnComputerVersion\\Course_Lab_2\\Assignment2\\board.jpg");
    Mat res;

    if(lena.empty() || board.empty()){
        cout << "Load image fail!" << endl;
        exit(0);
    }

    points4img lenaData;
    lenaData.img = lena;

    //标记lena图角点，上左开始顺时针
    lenaData.points.push_back(Point2f(0, 0));
    lenaData.points.push_back(Point2f(lena.cols -1, 0));
    lenaData.points.push_back(Point2f(lena.cols - 1, lena.rows - 1));
    lenaData.points.push_back(Point2f(0, lena.rows - 1));

    points4img boardData;
    boardData.img = board;

    //拷贝广告图
    res = board.clone();

    imshow("board", boardData.img);
    setMouseCallback("board", mouseHandler, &boardData);
    waitKey(0);

    //计算单应矩阵
    Mat h = findHomography(lenaData.points, boardData.points);
    warpPerspective(lenaData.img, res, h, res.size());

    //提取鼠标点击的四个点
    Point pts_data[4];
    for(int i=0; i<4; i++){
        pts_data[i] = boardData.points[i];
    }

    //依据角点填充
    fillConvexPoly(board, pts_data, 4, Scalar(0), CV_AA);

    board = board + res;
    imshow("a", board);
    waitKey(0);
    return 0;
}
