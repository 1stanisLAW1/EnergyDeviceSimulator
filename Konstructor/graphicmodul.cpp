#include "graphicmodul.h"
#include "qpainter.h"
#define ModulHeight 20


GraphicModul::GraphicModul(Moduls *m)
{
    modul = m;
}

GraphicModul::~GraphicModul()
{

}

QRectF GraphicModul::boundingRect() const
{
    QRectF rect(0, 0, 60,modul->len()/10);
    return rect;
}

void GraphicModul::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (modul->name()=="модуль KOR-BAT"){
        painter->setBrush(Qt::green);
    }
    else if(modul->name()=="модуль стыковочный"){
        painter->setBrush(Qt::red);
    }
    else if(modul->name()=="модуль Генератора"){
        painter->setBrush(Qt::yellow);
    }
    else if(modul->name()=="Модуль KOR-MIG2"){
        painter->setBrush(Qt::blue);
    }
    else if(modul->name()=="Модулятор"){
        painter->setBrush(Qt::darkCyan);
    }
    else if(modul->name()=="модуль МРК"){
        painter->setBrush(Qt::darkRed);
    }
    else if(modul->name()=="модуль Резистивиметра"){
        painter->setBrush(Qt::darkMagenta);
    }
    else if(modul->name()=="модуль телескопический"){
        painter->setBrush(Qt::gray);
    }
    else if(modul->name()=="Модуль KOR-TM3"){
        painter->setBrush(Qt::red);
    }
    else if(modul->name()=="Удлинитель резистивиметра"){
        painter->setBrush(Qt::darkRed);
    }
    QRectF rect(0,0,20,modul->len()/10);
    painter->drawRect(rect);
    QList<Point>listPoint = modul->getPoints();
    for (int i = 0;i<listPoint.size();i++){
        painter->drawLine(0,listPoint.at(i).lent()/10,ModulHeight,listPoint.at(i).lent()/10);
    }
}

QString GraphicModul::info() const
{
    return QString("Modul %1 pos %2").arg(modul->name()).arg(scenePos().x());
}
