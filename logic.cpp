#include "logic.h"
#include "QSerialPortInfo"
#include "qdebug.h"

Logic::Logic() {

}

QVector<QString> Logic::showPortsName()
{
    QVector<QString>portsName;
    qDebug() << "Available serial ports:";
    if(QSerialPortInfo::availablePorts().empty()){
        QString error = "Available serial ports: \n No ports";
        qDebug()<<error.trimmed();
    }
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &portInfo : serialPortInfos) {
        qDebug()<<portInfo.portName();
        portsName.append(portInfo.portName());
    }
    return portsName;
}

