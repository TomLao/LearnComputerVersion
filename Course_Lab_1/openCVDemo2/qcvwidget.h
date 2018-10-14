#ifndef QCVWIDGET_H
#define QCVWIDGET_H

#include <QWidget>
#include <QThread>

namespace Ui {
class QCvWidget;
}

class QCvWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::QCvWidget *ui;
    QThread *thread;

    void setup();

public:
    explicit QCvWidget(QWidget *parent = 0);
    ~QCvWidget();

signals:
    void sendSetup(int device);
    void sendToggleStream();

private slots:
    void receiveFrame(QImage frame);
    void receiveToggleStream();
};

#endif // QCVWIDGET_H
