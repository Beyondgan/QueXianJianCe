#ifndef WIDGET_H
#define WIDGET_H

#include <QGraphicsScene>
#include <QWidget>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_Open_image_clicked();

    void on_pushButton_Gray_image_clicked();

    void on_pushButton_Gaus_image_clicked();

    void on_pushButton_Blur_image_clicked();

    void on_pushButton_Media_image_clicked();

    void on_pushButton_Bilateral_image_clicked();

    void on_pushButton_Sobel_image_clicked();

    void on_pushButton_Canny_image_clicked();

    void on_pushButton_Erode_image_clicked();

    void on_pushButton_Dilate_image_clicked();

    void on_pushButton_morphologyEx_image_open_clicked();

    void on_pushButton_morphologyEx_image_close_clicked();

    void on_pushButton_morphologyEx_image_gradient_clicked();

    void on_pushButton_Binary_image_clicked();

    void on_pushButton_Adaptive_image_clicked();

    void on_pushButton_Otsu_image_clicked();

    void on_pushButton_calcHist_image_clicked();

    void on_pushButton_equalizeHist_image_clicked();

    void on_pushButton_compareHist_image_clicked();

    void on_pushButton_createCLAHE_image_clicked();

private:
    Ui::Widget *ui;
    cv::Mat image;//原图
    cv::Mat image_gray;//灰度图
    cv::Mat image_Filter;//滤波图
    cv::Mat image_Edge_detection;//边缘检测图
    cv::Mat image_Morphology;//形态学图
    cv::Mat image_Threshold;//阈值图
    cv::Mat image_Histogram;//直方图
    cv::Mat data_Histogram;//直方图
    // cv::Mat image_gaus;
    // cv::Mat image_blur;
    // cv::Mat image_median;
    // cv::Mat image_bilateral;
};
#endif // WIDGET_H
