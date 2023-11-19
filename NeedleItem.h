#ifndef NEEDLEITEM_H
#define NEEDLEITEM_H


#include <QObject>
#include <QGraphicsItem>
#include <QImage>
#include <QPropertyAnimation>


class NeedleItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    Q_PROPERTY(qreal anagle READ rotation WRITE setRotation)
public:
    explicit NeedleItem(QObject *parent = nullptr);



    virtual QRectF boundingRect() const ;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

    void setStartTime();

    void setEndTime();




signals:

public slots:

private:
    QImage image;
    QPropertyAnimation *animation;
};

#endif // DISKITEM_H
