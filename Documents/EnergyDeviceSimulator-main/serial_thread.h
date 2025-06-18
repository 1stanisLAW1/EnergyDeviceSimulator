#ifndef SERIAL_THREAD_H
#define SERIAL_THREAD_H

#include "logiwrite.h"
#include "mainfunctions.h"
#include "qserialport.h"
#include "qtimer.h"
#include <QObject>

class Serial_Thread : public QObject
{
    Q_OBJECT
public:
    explicit Serial_Thread(QObject *parent = nullptr);
    void read_and_write_comands();
    void close_port();
    void appendList(QList<QWidget*>list);
    void count_packs();

    void start_timer();

public slots:
    void show_port(const QString &portName,const QString &baund,const QString &parity,const QString&stop,const QString s);
    void update_port(const QString &portName,const QString &baund,const QString &parity,const QString&stop,const QString s);
    void stop_timer();
    void valid(){
        emit valid_signal();
    }
    void update();


signals:
    void signal_setTime(QString);
    void packs_count(int);
    void valid_signal();
private:
    LogiWrite*lw;
    Mainfunctions*mF;
    int counterPacks = 0;
    QByteArray answer;
    QString str;
    QList<QString>list_answer;
    QSerialPort* serial_port;
    QTimer*timer;
    QTimer*timer_2;
    QTimer*timer_3;
  //  std::atomic<bool> m_stopRequested{false};
    bool time = false;
};

#endif // SERIAL_THREAD_H
