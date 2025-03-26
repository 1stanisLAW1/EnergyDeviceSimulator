#ifndef TELESYSTEM_H
#define TELESYSTEM_H

#include "graphicmodul.h"
#include "graphicsscene.h"
#include "moduls.h"
#include "point.h"
#include "qslider.h"
#include <QObject>
#include <QGraphicsScene>

class Telesystem : public QObject
{
    Q_OBJECT
public:
    explicit Telesystem(QObject *parent = nullptr);
    void setScene(GraphicsScene *s);
    bool appendModul(Moduls m);
    QList <Moduls*> getModuls() const{return mModules;};
    Moduls::TypeCoupling lastCoupling();
    void info();
    void graphicsInfo() const;
    void drawModul(Moduls*);
    double fullLen()const;
    double lenToModule(int) const;
    void pointInfo();
    QList<Moduls*> mModules;
signals:
    void updated();
    void txtEd(int,QString);
public slots:
    void acceptsItem(QList<QGraphicsItem*>);
    void dialog();
    void remove();
    void clearScene();
    void pointShow(QList<QGraphicsItem*>);
private:
    QList<GraphicModul*>gMod;
    QMap<GraphicModul*, QList<QGraphicsItem*>> mAdditionalItems;
    QGraphicsLineItem* lineLeft = nullptr;
    QGraphicsLineItem* lineRight = nullptr;
    GraphicsScene *graphicsScene = nullptr;
    QSlider *slider;
    int modifiedIndex = -1;
    int fulLen = 0;
};


#endif // TELESYSTEM_H
