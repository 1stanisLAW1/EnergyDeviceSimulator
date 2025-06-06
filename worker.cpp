#include "worker.h"

Worker::Worker(QObject *parent)
    : QObject{parent}
{}

void Worker::process()
{
    widg = new UpWidg();

    if(widg == nullptr){
        emit finished(false);
        return;
    }
    widg->upWidget();

    emit finished(true);
}
