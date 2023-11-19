#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QImage>
#include <QGraphicsScene>
#include "DiskItem.h"
#include "NeedleItem.h"

class Graphicsview : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Graphicsview(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent* pe);
    void setImage(const QImage& pic);
    void resizeEvent(QResizeEvent* re);

    QImage makeArgb32Image(QImage picImage);
    QImage makeCircleImage(QImage argb32Image);
    QImage makeDiskImage(QImage circleImage);

    void setStartTime();

    void setEndTime();

signals:

public slots:

private:
    QImage image;
    QGraphicsScene *scene;
    DiskItem *diskItem;
    NeedleItem *needleItem;

};

#endif // GRAPHICSVIEW_H
