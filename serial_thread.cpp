#include "serial_thread.h"
#include "qdatetime.h"
#include "qelapsedtimer.h"
#include "qtimer.h"

Serial_Thread::Serial_Thread(QObject *parent)
    : QObject{parent}
{
    lw= new LogiWrite(this);
    mF = new Mainfunctions(this);
    serial_port = new QSerialPort(this);

    timer = new QTimer(this);
    timer_2 = new QTimer(this);

    connect(mF,&Mainfunctions::valid,this,&Serial_Thread::valid);

    connect(timer, &QTimer::timeout, this, &Serial_Thread::read_and_write_comands);
    connect(timer_2, &QTimer::timeout, this, [this]() {
        emit packs_count(counterPacks);
    });
}

void Serial_Thread::read_and_write_comands()
{
    QByteArray requestData;
    QElapsedTimer packetTimer;
    packetTimer.start();

    // Чтение данных с учетом временных пауз между пакетами (3 мс)
    while (packetTimer.elapsed() < 50) { // Максимальное время ожидания ответа 50 мс
        if (serial_port->waitForReadyRead(3)||requestData.size()!=7) { // Ожидание данных с учетом паузы между пакетами
            requestData += serial_port->readAll();
            packetTimer.restart();

            // Проверка конца пакета по байту 0xFF
            if (requestData.endsWith('\xFF')) {
                break;
            }
        }
    }
    if(requestData.isEmpty()){return;}

     quint8 c = static_cast<quint8>(requestData[2]);
    // Проверка минимальной длины (7 байт) и байта конца пакета (0xFF)
    //if (requestData.size() >= 7 && requestData.size()<=15 && static_cast<quint8>(requestData[requestData.size() - 1]) == 0xFF) {
     if((requestData.size() > 7 && requestData.size()<=15 && c == 0x19)||(requestData.size() == 7)){
        // Проверка адреса отправителя (0x89 или 0xF0 для широковещательных)
        quint8 address = static_cast<quint8>(requestData[0]);
        if (address == 0x89 || address == 0xF0) {
            lw->appendList(requestData.toHex(), 1);

            quint8 commandNum = static_cast<quint8>(requestData[2]);
            if (commandNum == 0x19){
                emit signal_setTime( mF->command_0x19(requestData));
                time = true;
            }
            else{
                answer = mF->answer_command_v2(requestData);

                serial_port->write(answer);

                if(!serial_port->waitForBytesWritten(10)) {
                    str = QTime::currentTime().toString("HH:mm:ss:zzz")+" - "+answer.toHex()+" - 0";
                    list_answer.append(str);
                    qDebug()<<answer;
                } else {
                    serial_port->flush();
                    str = QTime::currentTime().toString("HH:mm:ss:zzz")+" - "+answer.toHex()+" - 1";
                    counterPacks++;
                    list_answer.append(str);
                }
            }

        } else {
            lw->appendList(requestData.toHex(), 0);
        }
    } else if (!requestData.isEmpty()) {
        lw->appendList(requestData.toHex(), 0);
    }
    else{
        lw->appendList(requestData.toHex(), 0);
    }
}

void Serial_Thread::show_port(const QString &portName, const QString &baund, const QString &parity, const QString &stop, const QString adress)
{
    serial_port->setPortName(portName);
    serial_port->setBaudRate(baund.toInt());
    if(parity=="NONE"){
        serial_port->setParity(QSerialPort::Parity::NoParity);
    }else if(parity=="EVEN"){
        serial_port->setParity(QSerialPort::Parity::EvenParity);
    }
    else if(parity=="ODD"){
        serial_port->setParity(QSerialPort::Parity::OddParity);
    }
    else if(parity=="MARK"){
        serial_port->setParity(QSerialPort::Parity::MarkParity);
    }
    else if(parity=="SPACE"){
        serial_port->setParity(QSerialPort::Parity::SpaceParity);
    }

    if(stop=="1"){
        serial_port->setStopBits(QSerialPort::StopBits::OneStop);
    }else if(stop=="2"){
        serial_port->setStopBits(QSerialPort::StopBits::TwoStop);
    }
    serial_port->setDataBits(QSerialPort::DataBits::Data8);

    if (!serial_port->isOpen()) {
        qDebug()<<"Последовательный порт закрыт";
        if (!serial_port->open(QIODevice::ReadWrite)) {
            qDebug() << "Ошибка открытия порта:" << serial_port->errorString();
            return;
        }
    }

    counterPacks = 0;
    mF->clearVec();
    mF->setAdres(adress);
    lw->clear_logs();
}

void Serial_Thread::update_port(const QString &portName, const QString &baund, const QString &parity, const QString &stop, const QString adress)
{
    serial_port->setPortName(portName);
    serial_port->setBaudRate(baund.toInt());
    if(parity=="NONE"){
        serial_port->setParity(QSerialPort::Parity::NoParity);
    }else if(parity=="EVEN"){
        serial_port->setParity(QSerialPort::Parity::EvenParity);
    }
    else if(parity=="ODD"){
        serial_port->setParity(QSerialPort::Parity::OddParity);
    }
    else if(parity=="MARK"){
        serial_port->setParity(QSerialPort::Parity::MarkParity);
    }
    else if(parity=="SPACE"){
        serial_port->setParity(QSerialPort::Parity::SpaceParity);
    }

    if(stop=="1"){
        serial_port->setStopBits(QSerialPort::StopBits::OneStop);
    }else if(stop=="2"){
        serial_port->setStopBits(QSerialPort::StopBits::TwoStop);
    }
    serial_port->setDataBits(QSerialPort::DataBits::Data8);

    if (!serial_port->isOpen()) {
        qDebug()<<"Последовательный порт закрыт";
        if (!serial_port->open(QIODevice::ReadWrite)) {
            qDebug() << "Ошибка открытия порта:" << serial_port->errorString();
            return;
        }
    }

    mF->clearVec();
    mF->setAdres(adress);
}

void Serial_Thread::close_port()
{
    qDebug()<<"порт закрыт";
    mF->clearVec();
    serial_port->close();
    lw->write_answer(list_answer);
    lw->write_request();
}

void Serial_Thread::appendList(QList<QWidget *> list)
{
    mF->appendList(list);
    lw->saveConfig(list);
}

void Serial_Thread::count_packs()
{
    emit packs_count(counterPacks);
}

void Serial_Thread::start_timer()
{
    timer->start(15);
    timer_2->start(15);
}

void Serial_Thread::stop_timer()
{
    timer->stop();
    timer_2->stop();
    close_port();
    count_packs();
}

void Serial_Thread::update()
{
    timer->stop();
    timer_2->stop();
    close_port();
    start_timer();
}
