#include "qcvwidget.h"
#include "ui_qcvwidget.h"
#include "opencvworker.h"
#include <QTimer>

QCvWidget::QCvWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QCvWidget)
{
    ui->setupUi(this);
    ui->labelView->setScaledContents(true);
    setup();
}

QCvWidget::~QCvWidget()
{
    thread->quit();
    while(!thread->isFinished());

    delete thread;
    delete ui;
}

void QCvWidget::setup()
{
    thread = new QThread();
    OpenCvWorker *worker = new OpenCvWorker();
    QTimer *workerTrigger = new QTimer();
    workerTrigger->setInterval(1);

    connect(workerTrigger, SIGNAL(timeout()), worker, SLOT(receiveGrabFrame()));
    connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), workerTrigger, SLOT(deleteLater()));
    connect(this, SIGNAL(sendSetup(int)), worker, SLOT(receiveSetup(int)));
    connect(this, SIGNAL(sendToggleStream()), worker, SLOT(receiveToggleStream()));
    connect(ui->pushButtonPlay, SIGNAL(clicked(bool)), this, SLOT(receiveToggleStream()));
    connect(ui->checkBoxEnableBinaryThreshold, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableBinaryThreshold()));
    connect(ui->spinBoxBinaryThreshold, SIGNAL(valueChanged(int)), worker, SLOT(receiveBinaryThreshold(int)));
    connect(worker, SIGNAL(sendFrame(QImage)), this, SLOT(receiveFrame(QImage)));
    connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), workerTrigger, SLOT(deleteLater()));

    workerTrigger->start();
    worker->moveToThread(thread);
//    worker->deleteLater();
    workerTrigger->moveToThread(thread);
//    workerTrigger->deleteLater();

    thread->start();

    emit sendSetup(0);
}

void QCvWidget::receiveFrame(QImage frame)
{
    ui->labelView->setPixmap(QPixmap::fromImage(frame));
}

void QCvWidget::receiveToggleStream()
{
    if(!ui->pushButtonPlay->text().compare(">")) ui->pushButtonPlay->setText("||");
    else ui->pushButtonPlay->setText(">");

    emit sendToggleStream();
}
