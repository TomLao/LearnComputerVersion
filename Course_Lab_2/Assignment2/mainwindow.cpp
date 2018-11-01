//
// Created by heze on 18-11-1.
//

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QMouseEvent>
#include <QtCore/QTextCodec>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image File"), "../Course_Lab_2/Assignment2/board.jpg", tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    QTextCodec *code = QTextCodec::codecForName("utf8");
    std::string name = code->fromUnicode(filename).data();
    billboard = imread(name);
    cvtColor(billboard, billboard, CV_BGR2RGB);

    boardData.img = billboard.clone();
    boardData.points.clear();

    img = QImage((const uchar*)(billboard.data), billboard.cols, billboard.rows, billboard.cols*billboard.channels(), QImage::Format_RGB888);
    ui->labelView_billboard->setPixmap(QPixmap::fromImage(img));
}


void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image File"), "../Course_Lab_2/Assignment2/lena.jpg", tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    QTextCodec *code = QTextCodec::codecForName("utf8");
    std::string name = code->fromUnicode(filename).data();
    lena = imread(name);
    cvtColor(lena, lena, CV_BGR2RGB);

    lenaData.img = lena.clone();
    //标记lena图角点，上左开始顺时针，emplace_back是在C++11中对push_back的改进版
    lenaData.points.emplace_back(Point2f(0, 0));
    lenaData.points.emplace_back(Point2f(lena.cols -1, 0));
    lenaData.points.emplace_back(Point2f(lena.cols - 1, lena.rows - 1));
    lenaData.points.emplace_back(Point2f(0, lena.rows - 1));

    img = QImage((const uchar*)(lena.data), lena.cols, lena.rows, lena.cols*lena.channels(), QImage::Format_RGB888);
    ui->labelView_lena->setPixmap(QPixmap::fromImage(img));
}


void MainWindow::on_pushButton_3_clicked()
{
    //temp用于在全黑的billboard图像下，显示变形的lena图
    Mat temp = billboard.clone();

    //计算单应矩阵
    Mat h = findHomography(lenaData.points, boardData.points);
    warpPerspective(lenaData.img, temp, h, temp.size());

    //提取鼠标点击的四个点
    Point pts_data[4];
    for(int i=0; i<4; i++){
        pts_data[i] = boardData.points[i];
    }

    //依据角点填充，res广告牌中依据点击角点，挖出其中内容
    fillConvexPoly(boardData.img, pts_data, 4, Scalar(0), CV_AA);
    //相加得到组合结果res
    temp = boardData.img + temp;

    cvtColor(temp, temp, CV_BGR2RGB);
    img = QImage((const uchar*)(temp.data), temp.cols, temp.rows, temp.cols*temp.channels(), QImage::Format_RGB888);
    ui->labelView_billboard->setPixmap(QPixmap::fromImage(img));
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // 单击事件，如果是鼠标左键按下
    int x, y;
    if (event->button() == Qt::LeftButton){
        //计算，鼠标点击位置与billboard的相对位置
        x = event->pos().x() - ui->labelView_billboard->x();
        y = event->pos().y() - ui->labelView_billboard->y();

        QString msg=QString("%1,%2").arg(x).arg(y);
        ui->label->setText(msg);

        cout << x << " rows" << boardData.img.cols << endl ;
        cout << y << " cols" << boardData.img.rows << endl;

        circle(boardData.img, Point(x, y), 1, Scalar(0, 0, 255), 5, CV_AA);

        //TODO:每次点击都更新显示红点，这个颜色的转化有点问题啊。。。。
        cvtColor(boardData.img, boardData.img, CV_BGR2RGB);
        img = QImage((const uchar*)(boardData.img.data), boardData.img.cols, boardData.img.rows, boardData.img.cols*boardData.img.channels(), QImage::Format_RGB888);
        ui->labelView_billboard->setPixmap(QPixmap::fromImage(img));

        if(x > 0 && y > 0 && x < boardData.img.cols && y < boardData.img.rows && boardData.points.size() < 4){
            //加入点向量
            boardData.points.emplace_back(Point2f(x, y));
            cout << "vector:" << Point2f(x, y) << endl;
        }
    }
//    右键按下
//    else if (event->button() == Qt::RightButton){}
//    中键按下
//    else if (event->button() == Qt::MidButton){}
}
