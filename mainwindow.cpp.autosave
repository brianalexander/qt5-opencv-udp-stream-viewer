#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateFrame(cv::Mat frame) {
    // Set the color table (used to translate colour indexes to qRgb values)
    QVector<QRgb> colorTable;
    for (int i=0; i<256; i++)
        colorTable.push_back(qRgb(i,i,i));
    // Create QImage with same dimensions as input Mat
    QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    img.setColorTable(colorTable);

    ui->videoFrame->setPixmap(QPixmap::fromImage(img));
}

