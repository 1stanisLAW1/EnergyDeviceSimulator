#include "moduls.h"
#include "qdebug.h"

Moduls::Moduls(const QString name, const int length, const int maxL, const int minL)
{
    mName = name;
    mMinLength = minL;
    mCurrentLength = length;
    mMaxLength = maxL;
}

QString Moduls::name() const{return mName;}

QList<Point> Moduls::getPoints() const{return mPoints;}

bool Moduls::setUpDown(TypeCoupling up, TypeCoupling down)
{
    bool ok = true;
    if(ok){
        mUpCoupling = up;
        mDownCoupling = down;
    }
    return ok;
}

void Moduls::addPoint(Point m)
{
    if(m.lent()>=len()){
        return;
    }
    mPoints.append(m);
}

Moduls::TypeCoupling Moduls::upCoupling() const
{
    return mUpCoupling;
}

Moduls::TypeCoupling Moduls::downCoupling() const
{
    return mDownCoupling;
}

bool Moduls::exemCoupling(TypeCoupling type1, TypeCoupling type2)
{
    qDebug() << "Moduls::exemCoupling" << type2str(type1) << type2str(type2);
    switch (type1) {
    case Moduls::TypeCoupling::None:
        return type2 == Moduls::TypeCoupling::TOP;
        break;
    case Moduls::TypeCoupling::KORVET_MRKm:
        return type2 == Moduls::TypeCoupling::KORVET_MRKp;
        break;
    case Moduls::TypeCoupling::KORVET_M:
        return type2 == Moduls::TypeCoupling::KORVET_P;
        break;
    case Moduls::TypeCoupling::KORVET_MRKp:
        return type2 == Moduls::TypeCoupling::KORVET_MRKm;
        break;
    case Moduls::TypeCoupling::KORVET_P:
        return type2 == Moduls::TypeCoupling::KORVET_M;
        break;
    default:
        break;
    }
    return false;
}

bool Moduls::variableLength() const
{
    return mMinLength!=mMaxLength;
}

bool Moduls::setCurrentLength(int l)
{
    if(!variableLength()){return false;}
    mCurrentLength = l;
    return true;
}

void Moduls::setMinMax(int min, int max)
{
    mMaxLength = max;
    mMinLength=min;
}

QString Moduls::infoMod()
{
    QString str = QString("Modul-%1 /n length-%2 min-%3 max-%4")
    .arg(name())
        .arg(len())
        .arg(minL())
        .arg(maxL());
    return str;
}

QString type2str(Moduls::TypeCoupling type)
{
    QString ret;
    switch (type) {
    case Moduls::TypeCoupling::None : ret = "None";break;
    case Moduls::TypeCoupling::TOP : ret = "TOP";break;
    case Moduls::TypeCoupling::KORVET_MRKm:ret = "KORVET_MRKm"; break;
    case Moduls::TypeCoupling::KORVET_MRKp:ret = "KORVET_MRKp"; break;
    case Moduls::TypeCoupling::KORVET_M:ret = "KORVET_M"; break;
    case Moduls::TypeCoupling::KORVET_P:ret = "KORVET_P"; break;
    case Moduls::TypeCoupling::BOTTOM:     ret = "BOTTOM"; break;
    default:
        ret = QString("error %1").arg(static_cast<int>(type));
    }
    return ret;
}
