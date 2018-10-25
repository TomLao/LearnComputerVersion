#include<opencv2/opencv.hpp>
#include<cv.h>
using namespace cv;
using namespace std;

int main(){
    VideoCapture capture(0);
    Mat img;
    double fps, t = 0;
    char str[10];//String of fps
    while(1){

        //getTickCount():计数设备从开始运行的毫秒
        //getTickFrequency():返回每秒的计时周期数
        //t:表示代码运行时间，是fps的倒数

        t = (double)getTickCount();
        Mat frame;
        if(waitKey(30) == 30){
            break;
        }
        capture >> frame;
        if(frame.empty()){
            printf("No captured frame!");
        }
        else{
            t = ((double)getTickCount() - t) / getTickFrequency();
            fps = 1.0/t;
            sprintf(str, "FPS:%.2f, width:%d, height:%d", fps, img.rows, img.cols);
            putText(frame, str, Point(100,100), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 255));

            //画十字线
            line(frame, Point(img.cols*0.618, 0), Point(img.cols*0.618, img.rows), Scalar(255, 255, 255));  //竖直线
            line(frame, Point(0, img.rows*0.618), Point(img.cols, img.rows*0.618), Scalar(255, 255, 255));  //横直线
            //画圆
            circle(frame, Point(img.cols*0.618, img.rows*0.618), 100, Scalar(255, 255, 255));

            cvtColor(frame, img, CV_BGR2GRAY);
            imshow("color", frame);
            imshow("gray", img);
        }
    }
    return 0;
}
