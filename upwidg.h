#ifndef UPWIDG_H
#define UPWIDG_H

#include "logic.h"
#include "logiwrite.h"
#include "mainfunctions.h"
#include "qcombobox.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "qserialport.h"
#include <QObject>

class UpWidg : public QObject
{
    Q_OBJECT
public:
    explicit UpWidg(QObject *parent = nullptr);
    ~UpWidg();
    QWidget* upWidget();
    void setItem_ComboBox();
    void updateValues();

    void read_commands();
    void appendList(QWidget*);
    QStringList openConfig();
    void saveConfig();
    void countPack();


private:
    QWidget*widget;
    QComboBox* comboBox_COM;
    QComboBox* comboBox_Bould;
    QComboBox* comboBox_Parity;
    QComboBox* comboBox_StopBits;
    QLabel* lblValue;
    QLineEdit* lblValue_2;
    QLabel* lbl_time;
    QPushButton* btnStart;

    QTimer*timer;
    QTimer*timer_2;
    QSerialPort serial;
    QThread *serialThread;

    Logic*log;
    Mainfunctions*mF;
    LogiWrite*lw;

    bool start = false;
    bool timerConnected;
    int counterPacks = 0;
    int count_2 = 0;
    QVector<QString>vec;
    QStringList value;
    QByteArray answer;
    QString str;

    QList<QString>list_answer;

signals:
    void updateRequested();
};

#endif // UPWIDG_H
