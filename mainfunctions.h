#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H

#include "logiwrite.h"
#include "qlineedit.h"
#include "qserialport.h"
#include <QObject>

class Mainfunctions : public QObject
{
    Q_OBJECT
public:
    explicit Mainfunctions(QObject *parent = nullptr);
    ~Mainfunctions();
    QString command_0x19(QByteArray requestData);
    QByteArray answer_command(QByteArray request,QSerialPort &serialPort);
    QByteArray command_01();
    QByteArray command_02();
    QByteArray command_03();
    QByteArray command_04();
    QByteArray command_05();
    QByteArray command_06();
    QByteArray command_07();
    QByteArray command_08();
    QByteArray command_0A();
    QByteArray command_0B();
    QByteArray command_0C();
    QByteArray command_0E();
    QByteArray command_18();
    QByteArray command_FC();

    void appendFloat(QByteArray &array, float value);

    void appendList(QList<QLineEdit*>list);

    void setting_Serial(QSerialPort &serialPort);

    void clearVec();
    void setAdres(QString);
private:
    QSerialPort serial;

    QVector<QString>values;
    int adress;
    QString str;
    LogiWrite* lw;
};

#endif // MAINFUNCTIONS_H
