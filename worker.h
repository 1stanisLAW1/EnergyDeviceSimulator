#ifndef WORKER_H
#define WORKER_H

#include "upwidg.h"
#include <QObject>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
public slots:
    void process();

signals:
    void finished(bool);
private:
    UpWidg*widg;
};

#endif // WORKER_H
