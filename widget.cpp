#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_Open_image_clicked()
{
    // 打开文件对话框选择图片
    QString filePath = QFileDialog::getOpenFileName(
        nullptr,
        "选择图片",
        "D:/Image",
        "图片文件 (*.png *.jpg *.jpeg *.bmp *.gif)"
        );

    if(filePath.isEmpty()) {
        qDebug()<<"没有读取到图片!";
    }

    // 使用OpenCV读取选中的图片
    image = cv::imread(filePath.toStdString());

    if(image.empty()) {
        // 如果图片读取失败
        qDebug()<<"无法读取图片，请检查文件是否有效！";
    }
    ui->graphicsView_Show_image->setMatImage(image);
}




void Widget::on_pushButton_Gray_image_clicked()
{
    if(image.type() == CV_8UC3) {
        cv::cvtColor(image, image_gray, cv::COLOR_BGR2GRAY);
    } else if(image.type() == CV_8UC1) {
        image_gray=image.clone();
    } else {
        qDebug()<<"转换失败！";
    }
    ui->graphicsView_Show_image->setMatImage(image_gray);
}


void Widget::on_pushButton_Gaus_image_clicked()
{
    cv::GaussianBlur(image_gray,image_Filter,cv::Size(9,9),0);
    ui->graphicsView_Show_image->setMatImage(image_Filter);
}


void Widget::on_pushButton_Blur_image_clicked()
{
    cv::blur(image_gray,image_Filter,cv::Size(9,9));
    ui->graphicsView_Show_image->setMatImage(image_Filter);
}


void Widget::on_pushButton_Media_image_clicked()
{
    cv::medianBlur(image_gray,image_Filter,9);
    ui->graphicsView_Show_image->setMatImage(image_Filter);
}


void Widget::on_pushButton_Bilateral_image_clicked()
{
    cv::bilateralFilter(image_gray,image_Filter,9,75,75);
    ui->graphicsView_Show_image->setMatImage(image_Filter);
}


void Widget::on_pushButton_Sobel_image_clicked()
{
    // Sobel edge detection
    cv::Mat sobelx, sobely;
    cv::Sobel(image_Filter, sobelx, CV_16S, 1, 0);
    cv::Sobel(image_Filter, sobely, CV_16S, 0, 1);
    cv::convertScaleAbs(sobelx, sobelx);
    cv::convertScaleAbs(sobely, sobely);
    cv::addWeighted(sobelx, 0.5, sobely, 0.5, 0, image_Edge_detection);  // 合并结果
    //cv::Sobel(image_blur, sobelxy, CV_16S, 1, 1, 5);
    ui->graphicsView_Show_image->setMatImage(image_Edge_detection);
}


void Widget::on_pushButton_Canny_image_clicked()
{
    cv::Canny(image_Filter, image_Edge_detection, 80, 200);  // 阈值1=100，阈值2=200
    ui->graphicsView_Show_image->setMatImage(image_Edge_detection);
}


void Widget::on_pushButton_Erode_image_clicked()
{
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::erode(image_Edge_detection, image_Morphology, kernel);
    ui->graphicsView_Show_image->setMatImage(image_Morphology);
}


void Widget::on_pushButton_Dilate_image_clicked()
{
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::dilate(image_Edge_detection, image_Morphology, kernel);
    ui->graphicsView_Show_image->setMatImage(image_Morphology);
}


void Widget::on_pushButton_morphologyEx_image_open_clicked()
{
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(image_Edge_detection, image_Morphology, cv::MORPH_OPEN, kernel);
    ui->graphicsView_Show_image->setMatImage(image_Morphology);
}


void Widget::on_pushButton_morphologyEx_image_close_clicked()
{
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(image_Edge_detection, image_Morphology, cv::MORPH_CLOSE, kernel);
    ui->graphicsView_Show_image->setMatImage(image_Morphology);
}


void Widget::on_pushButton_morphologyEx_image_gradient_clicked()
{
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(image_Edge_detection, image_Morphology, cv::MORPH_GRADIENT, kernel);
    ui->graphicsView_Show_image->setMatImage(image_Morphology);
}


void Widget::on_pushButton_Binary_image_clicked()
{
    cv::threshold(image_Filter, image_Threshold, 127, 255, cv::THRESH_BINARY);
    ui->graphicsView_Show_image->setMatImage(image_Threshold);
}


void Widget::on_pushButton_Adaptive_image_clicked()
{
    cv::adaptiveThreshold(image_Filter, image_Threshold, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, 2);
    ui->graphicsView_Show_image->setMatImage(image_Threshold);
}


void Widget::on_pushButton_Otsu_image_clicked()
{
    cv::threshold(image_Filter, image_Threshold, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    ui->graphicsView_Show_image->setMatImage(image_Threshold);
}


void Widget::on_pushButton_calcHist_image_clicked()
{
    //cv::Mat hist;
    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};
    cv::calcHist(&image_Filter, 1, 0, cv::Mat(), data_Histogram, 1, &histSize, &histRange);
    int hist_w = 512; // 直方图图像的宽度
    int hist_h = 400; // 直方图图像的高度
    int bin_w = cvRound((double)hist_w / histSize); // 每个bin在图像中的宽度
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(20, 20, 20)); // 创建一个黑色背景的图像用于绘制
    // 归一化直方图的值到 [0, hist_h] 区间，这样才能画在图上
    // `normalize` 函数会找到hist中的最大值，然后按比例缩放所有值
    cv::normalize(data_Histogram, data_Histogram, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

    // 绘制直方图的每个bin
    for (int i = 1; i < histSize; i++) {
        line(histImage,
             cv::Point(bin_w * (i - 1), hist_h - cvRound(data_Histogram.at<float>(i - 1))), // 上一个点
             cv::Point(bin_w * (i), hist_h - cvRound(data_Histogram.at<float>(i))),     // 当前点
             cv::Scalar(200, 200, 200), // 线条颜色 (浅灰色)
             2, 8, 0);
    }
    ui->graphicsView_Show_image->setMatImage(histImage);
}


void Widget::on_pushButton_equalizeHist_image_clicked()
{
    cv::equalizeHist(image_Filter, image_Histogram);
    ui->graphicsView_Show_image->setMatImage(image_Histogram);
}


void Widget::on_pushButton_compareHist_image_clicked()
{
    double compare = cv::compareHist(data_Histogram, data_Histogram, cv::HISTCMP_CORREL);
    qDebug()<<compare;
}



void Widget::on_pushButton_createCLAHE_image_clicked()
{
    // 自适应直方图均衡
    cv::Ptr<cv::CLAHE> clahe = createCLAHE(2.0, cv::Size(8, 8));  // 限制对比度 2.0，窗口大小 8×8
    clahe->apply(image_Filter, image_Histogram);
    ui->graphicsView_Show_image->setMatImage(image_Histogram);
}

