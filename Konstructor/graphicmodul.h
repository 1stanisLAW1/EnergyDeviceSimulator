#ifndef GRAPHICMODUL_H
#define GRAPHICMODUL_H

#include "moduls.h"
#include "qgraphicsitem.h"
#include "qslider.h"
#include "qstyleoption.h"

class GraphicModul : public QGraphicsItem
{
public:
    QSlider *slider;
    GraphicModul(Moduls*);
    ~GraphicModul();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QString info() const;
    // void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
private:
    Moduls *modul;
    bool selected =false;
};

#endif // GRAPHICMODUL_H
