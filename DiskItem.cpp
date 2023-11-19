#include "DiskItem.h"
#include <QPainter>

DiskItem::DiskItem(QObject *parent) : QObject(parent),
  animation (new QPropertyAnimation(this,"anagle",this))
{
    image.load(":/Image/disk.png");

    animation->setDuration(10000);   //执行一次动画所用的时间
    animation->setStartValue(0);   //起始角度
    animation->setEndValue(360);     //结束角度
    animation->setLoopCount(-1);   //动画循环次数，-1为无限循环
}

void DiskItem::setImage(const QImage &musicImage)
{
    image = musicImage;
    update();
}

QRectF DiskItem::boundingRect() const
{
    return QRectF( image.width() / -2.0, image.height()/ -2.0,image.width(),image.height());
}

void DiskItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawImage(boundingRect(),image);
}

void DiskItem::setStartTime()
{
    animation->start();
}

void DiskItem::setEndTime()
{
    animation->pause();
}
