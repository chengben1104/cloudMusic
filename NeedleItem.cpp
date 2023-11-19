#include "NeedleItem.h"
#include <Qpainter>


NeedleItem::NeedleItem(QObject *parent) : QObject(parent),
    animation (new QPropertyAnimation(this,"anagle",this))
  {
      image.load(":/Image/magnetNeedle.png");

      setTransformOriginPoint(-102,0);

      animation->setDuration(100);   //执行一次动画所用的时间

      animation->setLoopCount(1);   //动画循环次数，-1为无限循环
  }


  QRectF NeedleItem::boundingRect() const
  {
      return QRectF( image.width() / -2.0, image.height()/ -2.0,image.width(),image.height());
  }

  void NeedleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
      Q_UNUSED(option);
      Q_UNUSED(widget);
      painter->setRenderHint(QPainter::SmoothPixmapTransform);
      painter->drawImage(boundingRect(),image);
  }

  void NeedleItem::setStartTime()
  {
      animation->setStartValue(0);   //起始角度
      animation->setEndValue(20);     //结束角度
      animation->start();
  }

  void NeedleItem::setEndTime()
  {
      animation->setStartValue(20);   //起始角度
      animation->setEndValue(0);     //结束角度
      animation->pause();
  }
