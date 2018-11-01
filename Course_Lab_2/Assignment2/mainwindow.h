//
// Created by heze on 18-11-1.
//

#ifndef LEARNCOMPUTERVERSION_MAINWINDOW_H
#define LEARNCOMPUTERVERSION_MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace Ui
{
    class MainWindow;
}

struct usedata{
    vector<Point2f> points;
    Mat img;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();   //选billboard

    void on_pushButton_2_clicked(); //选lena

    void on_pushButton_3_clicked(); //运行合成

    void mousePressEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    Mat lena;
    Mat billboard;

    usedata lenaData;
    usedata boardData;

    QImage img;
};
#endif //LEARNCOMPUTERVERSION_MAINWINDOW_H
