#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QGraphicsView>
#include <QObject>
#include <opencv2/opencv.hpp>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QImage>
#include <QWheelEvent>
#include <QGraphicsScene>

class ImageViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = nullptr);

public:
    void setQImage(QImage);
    void setMatImage(cv::Mat );
    void setPixmap(QPixmap);
    void resetImage();
    QImage getQImage();
    cv::Mat getMatImage();
    QPixmap getPixmap();

protected:
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    void zoom(QPoint factor);
    void togglePan(bool pan, const QPoint &startPos = QPoint());
    void pan(const QPoint &panTo);
    void initShow();

private:
    QPixmap m_image;
    bool m_isPan;
    QPoint m_prevPan;
    QGraphicsScene *scene;
};

#endif // IMAGEVIEWER_H
