#ifndef MODULS_H
#define MODULS_H

#include "point.h"
#include "qlist.h"
#include <QObject>
class Moduls
{
public:
    Moduls(){};
    Moduls(const QString name, const int length,const int maxL,const int minL);
    int pointsCount() const{return mPoints.size();};
    QString name() const;
    int len() const{return mCurrentLength;}
    int minL() const {return mMinLength;}
    int maxL() const {return mMaxLength;}
    QList <Point> getPoints() const;
    enum class TypeCoupling{None,TOP,KORVET_MRKm,KORVET_MRKp,KORVET_M,KORVET_P,BOTTOM};
    bool setUpDown(TypeCoupling up,TypeCoupling down);
    void addPoint(Point);
    bool isValid() const{return !mName.isEmpty();}
    TypeCoupling upCoupling() const;
    TypeCoupling downCoupling() const;
    static bool exemCoupling(TypeCoupling type1, TypeCoupling type2);
    bool variableLength() const;
    bool setCurrentLength(int l);
    void setMinMax(int min,int max);
    QString infoMod();

private:
    int mMaxLength = 0;
    int mMinLength = 0;
    int mCurrentLength = 0;

    QString mName;
    QList <Point> mPoints;
    TypeCoupling mUpCoupling;
    TypeCoupling mDownCoupling;
    QString length();
};
QString type2str(Moduls::TypeCoupling type);

#endif // MODULS_H
