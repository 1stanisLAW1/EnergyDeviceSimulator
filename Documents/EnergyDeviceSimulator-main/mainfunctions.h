#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H

#include "logiwrite.h"
#include <QObject>

class Mainfunctions : public QObject
{
    Q_OBJECT
public:
    explicit Mainfunctions(QObject *parent = nullptr);
    ~Mainfunctions();
    QString command_0x19(QByteArray requestData);

    QByteArray answer_command_v2(QByteArray request);

    void appendFloat(QByteArray &array, float value);

    void appendList(QList<QWidget*>list);

    void clearVec();
    void setAdres(QString);

    bool isLatinAndDigits(const QString &str);
signals:
    void valid();

private:
    QVector<QString>values;
    int adress;
    QString str;
    LogiWrite* lw;
};

#endif // MAINFUNCTIONS_H
