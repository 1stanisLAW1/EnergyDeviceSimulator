#include "point.h"
#include "qdebug.h"
#define MAX_LEN 5000
#define MIN_LEN 0

Point::Point(const QString name, const QString nameM, const int len) {
    mName = name;
    mMnName = nameM;
    mLength = len;
}

void Point::setLen(int l)
{
    if(MAX_LEN<l||MIN_LEN>l){
        qDebug()<<__LINE__<<__FILE__<<"wrong len";
    }
    mLength = l;
}

void Point::setName(QString n)
{
    if(n.isEmpty()){
        return;
    }
    mName = n;
}

void Point::setMName(QString mn)
{
    if(mn.isEmpty()){
        return;
    }
    mMnName = mn;
}
