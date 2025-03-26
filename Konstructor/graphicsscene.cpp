#include "graphicsscene.h"
#include "qgraphicsitem.h"
#include "qgraphicssceneevent.h"
#include "qgraphicsview.h"
#include "telesystem.h"
#include <QDebug>
GraphicsScene::GraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::RightButton) {
        QPointF scenePos = mouseEvent->scenePos();
        QGraphicsScene::mousePressEvent(mouseEvent);
        if(!items(scenePos).isEmpty()){
            emit itemSelect(items(scenePos));
            qDebug()<<items(scenePos);
        }
        return;
    }
    else if(mouseEvent->button() == Qt::LeftButton){
        QPointF scenePos = mouseEvent->scenePos();
        QGraphicsScene::mousePressEvent(mouseEvent);
        if(!items(scenePos).isEmpty()){
            emit lkm(items(scenePos));
        }
    }
}
