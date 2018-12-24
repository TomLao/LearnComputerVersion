#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "worker.cpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    string src = "D://MyProject//LearnComputerVersion//FinalProject//test_video//solidYellowLeft.mp4";
    video=cv::VideoCapture(src);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getframe(){
    video>>src_frame; //从视频取帧
    if(!src_frame.data)
        timer->stop();//如果取不到数据，终止计时器
    cvtColor(src_frame, src_frame, CV_BGR2RGB);

    //TODO:写识别代码
    start(src_frame);

    qimg = QImage((const uchar*)src_frame.data,src_frame.cols,src_frame.rows, QImage::Format_RGB888); //简单地转换一下为Image对象，rgbSwapped是为了显示效果色彩好一些。
    //ui->label->clear();
    QPixmap fitpixmap = QPixmap::fromImage(qimg).scaled(576, 324, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
    ui->labelVideo->setPixmap(fitpixmap);
}


void MainWindow::on_buttonStart_clicked(){
//    Mat image = imread("D://MyProject//LearnComputerVersion//FinalProject//images//dark-shades.png");
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(getframe()));timer->start(42);//按照每秒24帧计算，每过42ms执行一次getframe
}
