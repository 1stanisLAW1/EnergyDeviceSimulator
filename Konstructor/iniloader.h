#ifndef INILOADER_H
#define INILOADER_H

#include "moduls.h"
#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QSettings>

class Iniloader : public QObject
{
    Q_OBJECT
public:
    explicit Iniloader(QObject *parent = nullptr);
    void extracted();
    Moduls createModel(QString filepath);
    QList<Moduls> createModuls(QString dirpath);
    Moduls::TypeCoupling convertStr2Type(const QString& str);
signals:
    void sendMessage(int,QString);
};

#endif // INILOADER_H
