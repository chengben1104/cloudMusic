#include "Graphicsview.h"
#include <Qpainter>
#include <QResizeEvent>

Graphicsview::Graphicsview(QWidget *parent) : QGraphicsView(parent),

    scene(new QGraphicsScene(this)),
    diskItem(new DiskItem(this)),
    needleItem(new NeedleItem(this))
{
    image.load(":/Image/logo.png");

    setScene(scene);
    scene->addItem(diskItem);
    scene->addItem(needleItem);
}

void Graphicsview::    paintEvent(QPaintEvent *pe)
{
    QPainter painter(viewport());

    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    painter.drawImage(rect(),image);

    QGraphicsView::paintEvent(pe);
}

void Graphicsview::setImage(const QImage &pic)
{
    //image = pic;

    //pic.save("1.png");


    QImage argb32Image = makeArgb32Image(pic);
    //argb32Image.save("2.png");
    QImage circleImage = makeCircleImage(argb32Image);

 //   circleImage.save("3.png");
    QImage diskImage = makeDiskImage(circleImage);
  // diskImage.save("4.png");
    diskItem->setImage(diskImage);

    update();
}

void Graphicsview::resizeEvent(QResizeEvent *re)
{
    int x = re->size().width();
    int y = re->size().height();

    scene->setSceneRect(x/-2.0,y/-2.0,x,y);
    diskItem->setPos(0,30);
    needleItem->setPos(-70,-190);
    QGraphicsView::resizeEvent(re);
}

QImage Graphicsview::makeArgb32Image(QImage picImage)
{
    QImage circle(":/Image/circleMask.png");

    //等比缩小专辑图片，并且改成ARGB32格式
    picImage = picImage.scaled( circle.size(),Qt::KeepAspectRatio);
    QImage fixedImage(circle.size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&fixedImage);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(fixedImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(picImage.rect(), picImage);
    painter.end();
    return fixedImage;
}

QImage Graphicsview::makeCircleImage(QImage argb32Image)
{
    //加载圆形图片
    QImage circle(":/Image/circleMask.png");
    //创建画家，以圆形图片为画板
     QPainter painter(&circle);
    //设置叠加模式
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    //把传进来的ARGB32图片绘制上去
    painter.drawImage(argb32Image.rect(),argb32Image);
    //结束绘制
    painter.end();
    //返回画板
    return circle;
}

QImage Graphicsview::makeDiskImage(QImage circleImage)
{
    //加载圆形图片
    QImage disk(":/Image/disk.png");
    //创建画家，以圆形图片为画板
     QPainter painter(&disk);
    //设置叠加模式
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    //把传进来的ARGB32图片绘制上去
    painter.drawImage(QPointF((disk.width() - circleImage.width()) / 2.0,
                             (disk.height() - circleImage.height()) / 2.0),circleImage);

    //结束绘制
    painter.end();
    //返回画板
    return disk;
}

void Graphicsview::setStartTime()
{
    diskItem->setStartTime();
    needleItem->setStartTime();
}

void Graphicsview::setEndTime()
{
    diskItem->setEndTime();
    needleItem->setEndTime();
}
