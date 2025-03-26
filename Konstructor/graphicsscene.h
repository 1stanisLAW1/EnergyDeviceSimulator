#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include "graphicmodul.h"
#include <QGraphicsScene>
#include <QObject>

class GraphicsScene: public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = nullptr);

    void addGraphicsItem(GraphicModul*);
signals:
    void itemSelect(QList<QGraphicsItem*>);
    void lkm(QList<QGraphicsItem*>);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
};

#endif // GRAPHICSSCENE_H
