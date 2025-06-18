#ifndef LOGICCOMMANDS_H
#define LOGICCOMMANDS_H

#include "qcontainerfwd.h"
#include "QVector"
class LogicCommands
{

public:
    LogicCommands();

    void message(QString);
    void message_2(QString);
signals:
    void emitMessage(QString);
private:
};

#endif // LOGICCOMMANDS_H
