#ifndef UPWIDG_H
#define UPWIDG_H

#include "logic.h"
#include "qcombobox.h"
#include "qdatetime.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "qthread.h"
#include "serial_thread.h"
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

    void appendList(QList<QWidget*>list);
    QStringList openConfig();
    void saveConfig();
    void set_config(QStringList);
    void default_config();
public slots:
    void set_time(QString);
    void valid(){
        if(start == true){
            btnStart->click();
        }else{
            start = false;
        }
    }
    void addTime();
signals:
    void startRequested(const QString &portName,const QString &baund,const QString &parity,const QString&stop,const QString s);
    void stopRequest();
    void updateRequested();
    void start_timer();
    void update(const QString &portName,const QString &baund,const QString &parity,const QString&stop,const QString s);


private:
    QWidget*widget;
    QComboBox* comboBox_COM;
    QComboBox* comboBox_Bould;
    QComboBox* comboBox_Parity;
    QComboBox* comboBox_StopBits;
    QPushButton* btnStart;
    QLabel* lblValue;
    QLineEdit* lblValue_2;
    QLabel* lbl_time;

    QThread* thread_port;

    Logic*log;
    Serial_Thread*st;

    bool start = false;
    bool timerConnected;
    int count_2 = 0;
    QVector<QString>vec;
    QStringList value;
    QByteArray answer;
    QString str;
    bool config_empty = false;
    QTimer* timer;
    QDateTime dt;
};

#endif // UPWIDG_H
