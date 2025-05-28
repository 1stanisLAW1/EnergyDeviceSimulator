#include "upwidg.h"
#include "logic.h"
#include "qboxlayout.h"
#include "qdatetime.h"
#include "qevent.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "qserialport.h"
#include "qwidget.h"

#include <QTimer>
#include <QFile>
#include <QThread>

UpWidg::UpWidg(QObject *parent)
    : QObject{parent}
{
    widget = new QWidget();
    log = new Logic();
    mF = new Mainfunctions();
    value = openConfig();

    timer = new QTimer();
    timer_2 = new QTimer();


    lw = new LogiWrite();


    comboBox_COM = new QComboBox();
    comboBox_Bould = new QComboBox();
    comboBox_Parity = new QComboBox();
    comboBox_StopBits = new QComboBox();
}

UpWidg::~UpWidg()
{
    delete widget;
    delete comboBox_COM;
    delete comboBox_Bould;
    delete comboBox_Parity;
    delete comboBox_StopBits;
    delete lblValue;
    delete lblValue_2;
    delete  lbl_time;
    delete btnStart;
    delete timer;
    delete timer_2;
    delete log;
    delete mF;
    delete lw;

}

 QWidget *UpWidg::upWidget()
{
    QLabel* lblCOM = new QLabel("COM port:");
    QLabel* lblBould = new QLabel("Bould rate:");
    QLabel* lblParity = new QLabel("Parity");
    QLabel* lblStopBits = new QLabel("Stop bits:");

    btnStart = new QPushButton("Start");
    QPushButton* btnUpdate = new QPushButton("Update values");

    QLabel* lbl_text = new QLabel("Date and time on the device");
    lbl_time = new QLabel("Nope");

   QLabel* lbl = new QLabel("Request Packs");
   lblValue = new QLabel("0");
    lblValue->setText("0");

   QLabel* lbl_2 = new QLabel("Адрес приёмника");
    lblValue_2 = new QLineEdit(value.at(0));
   lblValue_2->setFixedWidth(50);


    QVBoxLayout* vLayout = new QVBoxLayout();
    QHBoxLayout* hLayout_Up = new QHBoxLayout();
    QHBoxLayout* hLayout_Centr = new QHBoxLayout();
    QHBoxLayout* hLayout_Down = new QHBoxLayout();

    vLayout->addLayout(hLayout_Up);
    vLayout->addLayout(hLayout_Centr);
    vLayout->addLayout(hLayout_Down);

    hLayout_Up->addWidget(lblCOM);
    hLayout_Up->addWidget(lblBould);
    hLayout_Up->addWidget(lblParity);
    hLayout_Up->addWidget(lblStopBits);

    hLayout_Centr->addWidget(comboBox_COM);
    hLayout_Centr->addWidget(comboBox_Bould);
    hLayout_Centr->addWidget(comboBox_Parity);
    hLayout_Centr->addWidget(comboBox_StopBits);

    hLayout_Down->addWidget(btnStart);
    hLayout_Down->addWidget(btnUpdate);
    hLayout_Down->addWidget(lbl_text);
    hLayout_Down->addWidget(lbl_time);
   hLayout_Down->addWidget(lbl);
   hLayout_Down->addWidget(lblValue);

   hLayout_Down->addWidget(lbl_2);
    hLayout_Down->addWidget(lblValue_2);

    widget->setLayout(vLayout);


    connect(comboBox_COM,&QComboBox::currentIndexChanged,this,[this](){
        if(comboBox_COM->currentIndex()==0){
            serial.setPortName("COM1");
        }else{
        serial.setPortName(comboBox_COM->currentText());
        }
    });
    connect(comboBox_Bould,&QComboBox::currentIndexChanged,this,[this](){
        serial.setBaudRate(comboBox_Bould->currentText().toInt());
    });
    connect(comboBox_Parity,&QComboBox::currentIndexChanged,this,[this](){
        switch (comboBox_Parity->currentIndex()) {
        case 0:
            serial.setParity(QSerialPort::Parity::NoParity);
            break;
        case 1:
            serial.setParity(QSerialPort::Parity::EvenParity);
            break;
        case 2:
            serial.setParity(QSerialPort::Parity::OddParity);
            break;
        case 3:
            serial.setParity(QSerialPort::Parity::MarkParity);
            break;
        case 4:
            serial.setParity(QSerialPort::Parity::SpaceParity);
            break;
        default:
            break;
        }
    });
    connect(comboBox_StopBits,&QComboBox::currentIndexChanged,this,[this](){
        switch (comboBox_StopBits->currentIndex()) {
        case 0:
            serial.setStopBits(QSerialPort::StopBits::OneStop);
            break;
        case 1:
            serial.setStopBits(QSerialPort::StopBits::TwoStop);
            break;
        default:
            break;
        }
    });

    connect(btnUpdate, &QPushButton::clicked, this, [this]() {
        updateValues();
        emit updateRequested();
    });

   serial.setDataBits(QSerialPort::DataBits::Data8);

    connect(btnStart, &QPushButton::clicked, this,[this](){

       start =! start;

       if(start == 1){

           if (!serial.isOpen()) {
               if (!serial.open(QIODevice::ReadWrite)) {
                   qDebug() << "Ошибка открытия порта:" << serial.errorString();
                   timer->stop(); // Останавливаем таймер при ошибке
                   btnStart->setText("Start");
                   start = false;
                   return;
               }
           }

           timer->start(15);
           timer_2->start(20);
           list_answer.clear();
           lblValue->setText("0");
           counterPacks =0;
           mF->clearVec();
           emit updateRequested();
           mF->setAdres(lblValue_2->text());
           lw->clear_logs();
           btnStart->setText("Stop");
            connect(timer, &QTimer::timeout, this, &UpWidg::read_commands);
            connect(timer_2,&QTimer::timeout, this,&UpWidg::countPack);
       }else {
           timer->stop();
           timer_2->stop();
           btnStart->setText("Start");
           serial.close();
           countPack();
           count_2 = 0;
           lw->write_answer(list_answer);
           lw->write_request();
       }
    });

   setItem_ComboBox();

   updateValues();

return widget;
}

void UpWidg::setItem_ComboBox()
{
    comboBox_COM->clear();
    comboBox_Bould->clear();
    comboBox_Parity->clear();
    comboBox_StopBits->clear();

    QVector<int>ports_BoundValues= {9600,19200,38400,57600,115200};

    for(int i =0; i<ports_BoundValues.size();i++){
        comboBox_Bould->addItem(QString::number(ports_BoundValues.at(i)));
    }

    QVector<QString>ports_Parity = {"NONE","EVEN","ODD","MARK","SPACE"};

    for(int i =0; i<ports_Parity.size();i++){
        comboBox_Parity->addItem(ports_Parity.at(i));
    }

    QVector<QString>ports_StopBits={"1","2"};

    for(int i =0; i<ports_StopBits.size();i++){
        comboBox_StopBits->addItem(ports_StopBits.at(i));
    }

    serial.setPortName(comboBox_COM->currentText());
    serial.setBaudRate(value.at(1).toInt());

    comboBox_Bould->setCurrentText(value.at(1));
    comboBox_Parity->setCurrentText(value.at(2));
    comboBox_StopBits->setCurrentText(value.at(3));

    if(value.at(2)=="NoParity"){
        serial.setParity(QSerialPort::Parity::NoParity);
    }else if(value.at(2)=="EvenParity"){
        serial.setParity(QSerialPort::Parity::EvenParity);
    }
    else if(value.at(2)=="OddParity"){
        serial.setParity(QSerialPort::Parity::OddParity);
    }
    else if(value.at(2)=="MarkParity"){
        serial.setParity(QSerialPort::Parity::MarkParity);
    }
    else if(value.at(2)=="SpaceParity"){
        serial.setParity(QSerialPort::Parity::SpaceParity);
    }

    if(value.at(3)=="OneStop"){
         serial.setStopBits(QSerialPort::StopBits::OneStop);
    }else if(value.at(3)=="TwoStop"){
        serial.setStopBits(QSerialPort::StopBits::TwoStop);
    }
}

void UpWidg::updateValues()
{
    QVector<QString>ports_Name = log->showPortsName();

    if(ports_Name.empty()){comboBox_COM->clear();return;}

    comboBox_COM->clear();
    for(int i =0; i<ports_Name.size();i++){
        comboBox_COM->addItem(ports_Name.at(i));
    }
    saveConfig();
}

void UpWidg::read_commands()
{

    QByteArray requestData;
    QElapsedTimer packetTimer;
    packetTimer.start();

    // Чтение данных с учетом временных пауз между пакетами (3 мс)
    while (packetTimer.elapsed() < 50) { // Максимальное время ожидания ответа 50 мс
        if (serial.waitForReadyRead(3)) { // Ожидание данных с учетом паузы между пакетами
            requestData += serial.readAll();
            packetTimer.restart();

            // Проверка конца пакета по байту 0xFF
            if (requestData.endsWith('\xFF')) {
                break;
            }
        }
    }
    if(requestData.isEmpty()){return;}


    // Проверка минимальной длины (7 байт) и байта конца пакета (0xFF)
    if (requestData.size() >= 7 && requestData.size()<=15 && static_cast<quint8>(requestData[requestData.size() - 1]) == 0xFF) {
        // Проверка адреса отправителя (0x89 или 0xF0 для широковещательных)
        quint8 address = static_cast<quint8>(requestData[0]);
        if (address == 0x89 || address == 0xF0) {
            lw->appendList(requestData.toHex(), 1);

            quint8 commandNum = static_cast<quint8>(requestData[2]);
            if (commandNum == 0x19){
                lbl_time->setText(mF->command_0x19(requestData));
            }
            else{
                answer = mF->answer_command(requestData,serial);

                serial.write(answer);
                qDebug()<<answer.toHex()<<" - "<<answer.size();

                if(!serial.waitForBytesWritten(10)) {
                    str = QTime::currentTime().toString("HH:mm:ss:zzz")+" - "+answer.toHex()+" - 0";
                    list_answer.append(str);
                    qDebug()<<answer;
                } else {
                    serial.flush();
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

void UpWidg::appendList(QWidget *w)
{
    mF->appendList(w->findChildren<QLineEdit*>());
}

QStringList UpWidg::openConfig()
{
    QString executablePath = QCoreApplication::applicationDirPath();
    QString filePath = executablePath + "/../conf/config.txt";
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        QStringList result;
        bool found = false;
        if(file.size()==0){
            qDebug()<<"Файл пуст";
        }

        // Читаем файл построчно
        while (!stream.atEnd()) {
            QString line = stream.readLine().trimmed();

            if (line.startsWith("[main]")) {
                while (!stream.atEnd()) {
                    line = stream.readLine().trimmed();
                    if (line.isEmpty()) continue;

                    if (line.startsWith("Adres_Priem = ")) {
                        QString value = line.mid(13).trimmed();
                        if (!value.isEmpty()) {
                            result.append(value);
                            found = true;
                            qDebug() << "Adres_Priem:" << value;
                        }
                    }
                    else if (line.startsWith("Baund = ")) {
                        QString value = line.mid(8).trimmed();
                        if (!value.isEmpty()) {
                            result.append(value);
                            qDebug() << "Baund:" << value;
                        }
                    }
                    else if (line.startsWith("Parity = ")) {
                        QString value = line.mid(9).trimmed();
                        if (!value.isEmpty()) {
                            result.append(value);
                            qDebug() << "Parity:" << value;
                        }
                    }
                    else if (line.startsWith("Stop_Bits = ")) {
                        QString value = line.mid(12).trimmed();
                        if (!value.isEmpty()) {
                            result.append(value);
                            qDebug() << "Stop_Bits:" << value;
                        }
                    }
                }
            }
        }

        file.close();

        if (!found) {
            qDebug() << "Параметр adres_priem не найден в файле";
            return QStringList(); // Возвращаем пустую строку если параметр не найден
        }

        return result;
    } else {
        qDebug() << "Error: Не удалось открыть файл для чтения";
        return QStringList(); // Возвращаем пустую строку при ошибке
    }
}

void UpWidg::saveConfig()
{
    QString executablePath = QCoreApplication::applicationDirPath();
    QString filePath = executablePath + "/../conf/config.txt";
    QFile file(filePath);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        if(lblValue_2->text().isEmpty()) {
            qDebug() << "Поле не может быть пустым";
            return;
        }

        if(lblValue_2->text().mid(0,2) != "0x") {
            qDebug() << "Неверный формат";
            return;
        }

        // Записываем в config
        stream << "[main]\n";
        stream << "Adres_Priem = " << lblValue_2->text() << "\n";
        stream<< "Baund = "<<comboBox_Bould->currentText()<<"\n";
        stream<< "Parity = "<<comboBox_Parity->currentText()<<"\n";
        stream<< "Stop_Bits = "<<comboBox_StopBits->currentText()<<"\n";

        if(stream.status() != QTextStream::Ok) {
            qDebug() << "Ошибка записи данных: " << filePath << stream.status();
        }

        file.close();

        if(file.error() != QFile::NoError) {
            qDebug() << "Ошибка закрытия файла";
        }
    }
    else {
        qDebug() << "Не удалось открыть файл для записи";
    }
}

void UpWidg::countPack()
{
    lblValue->setText(QString::number(counterPacks));
}

