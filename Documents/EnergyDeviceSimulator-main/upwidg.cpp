#include "upwidg.h"
#include "logic.h"
#include "qboxlayout.h"
#include "qdatetime.h"
#include "qevent.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "qwidget.h"

#include <QTimer>
#include <QFile>
#include <QThread>

UpWidg::UpWidg(QObject *parent)
    : QObject{parent}
{
    widget = new QWidget();
    log = new Logic();
    st = new Serial_Thread();

    timer = new QTimer();

    QThread* thread = new QThread();
    st->moveToThread(thread);
    thread->start();


    comboBox_COM = new QComboBox();
    comboBox_Bould = new QComboBox();
    comboBox_Parity = new QComboBox();
    comboBox_StopBits = new QComboBox();

    connect(timer, &QTimer::timeout, this, &UpWidg::addTime);

    connect(st,&Serial_Thread::valid_signal,this,&UpWidg::valid);
    connect(st,&Serial_Thread::signal_setTime,this,&UpWidg::set_time);

    connect(this,&UpWidg::startRequested,st,&Serial_Thread::show_port);
    connect(this,&UpWidg::update,st,&Serial_Thread::update_port);
    connect(this, &UpWidg::start_timer, st, &Serial_Thread::start_timer, Qt::QueuedConnection);
    connect(this, &UpWidg::stopRequest, st, &Serial_Thread::stop_timer, Qt::QueuedConnection);

    connect(st, &Serial_Thread::packs_count, this, [this](int count) {
        lblValue->setText(QString::number(count));
    });

    setItem_ComboBox();
    value = openConfig();
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
    delete log;
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


    connect(btnUpdate, &QPushButton::clicked, this, [this]() {
        updateValues();
        emit updateRequested();
        emit update(comboBox_COM->currentText(),comboBox_Bould->currentText(),comboBox_Parity->currentText(),
                    comboBox_StopBits->currentText(),lblValue_2->text());
    });


    connect(btnStart, &QPushButton::clicked, this,[this,btnUpdate](){

        start =! start;

        if(start == 1){
            btnUpdate->click();
            emit startRequested(comboBox_COM->currentText(),comboBox_Bould->currentText(),comboBox_Parity->currentText(),
                              comboBox_StopBits->currentText(),lblValue_2->text());
            emit start_timer();
            lblValue->setText("0");
            btnStart->setText("Stop");

        }else {
            emit stopRequest();
            btnStart->setText("Start");
        }
    });
    btnUpdate->click();
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

void UpWidg::appendList(QList<QWidget *> list)
{
    st->appendList(list);
}

QStringList UpWidg::openConfig()
{
    QString executablePath = QCoreApplication::applicationDirPath();
    QString filePath = executablePath + "/conf/config.txt";

    QFile file(filePath);
    if (!file.exists() || file.size() == 0) {
        qDebug() << "Файл не существует или пуст, создаем конфиг по умолчанию";
        file.close();
        default_config();
    }

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        QStringList result;
        bool found = false;

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

        set_config(result);

        file.close();

        if (!found) {
            qDebug() << "Параметр adres_priem не найден в файле";
            return QStringList();
        }

        return result;
    } else {
        qDebug() << "Error: Не удалось открыть файл для чтения";
        return QStringList();
    }
}

void UpWidg::saveConfig()
{
    QString executablePath = QCoreApplication::applicationDirPath();
    QString filePath = executablePath + "/conf/config.txt";
    QFile file(filePath);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        if(lblValue_2->text().isEmpty()) {
            qDebug() << "Поле не может быть пустым";
            return;
        }

        if((lblValue_2->text().mid(0,2) != "0x")||(lblValue_2->text().mid(2,2) == "00"||lblValue_2->text().mid(2,2) == "01"||lblValue_2->text().mid(2,2) == "FE"||lblValue_2->text().mid(2,2) == "FF")) {
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

void UpWidg::set_config(QStringList config)
{
    if(comboBox_Parity->count()<=0||comboBox_COM->count()<=0||comboBox_Bould->count()<=0||comboBox_StopBits->count()<=0){
        qDebug()<<"Пусто";
    }
    for(int i = 0;i<comboBox_COM->count()-1;i++){
        int index = comboBox_COM->findText(config.at(0));
        if (index != -1) {  // если такое значение есть в comboBox
            comboBox_COM->setCurrentIndex(index);
        } else {
            qDebug() << "Значение" << config.at(0) << "не найдено в comboBox";
        }
    }
    for(int i = 0;i<comboBox_Bould->count()-1;i++){
        int index = comboBox_Bould->findText(config.at(1));
        if (index != -1) {  // если такое значение есть в comboBox
            comboBox_Bould->setCurrentIndex(index);
        } else {
            qDebug() << "Значение" << config.at(1) << "не найдено в comboBox";
        }
    }
    for(int i = 0;i<comboBox_Parity->count()-1;i++){
        int index = comboBox_Parity->findText(config.at(2));
        if (index != -1) {  // если такое значение есть в comboBox
            comboBox_Parity->setCurrentIndex(index);
        } else {
            qDebug() << "Значение" << config << "не найдено в comboBox";
        }
    }
    for(int i = 0;i<comboBox_StopBits->count()-1;i++){
        int index = comboBox_StopBits->findText(config.at(3));
        if (index != -1) {  // если такое значение есть в comboBox
            comboBox_StopBits->setCurrentIndex(index);
        } else {
            qDebug() << "Значение" << config.at(3) << "не найдено в comboBox";
        }
    }
}

void UpWidg::default_config()
{
    QString executablePath = QCoreApplication::applicationDirPath();
    QString filePath = executablePath + "/conf/config.txt";
    QFile file(filePath);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        // Записываем в config
        stream << "[main]\n";
        stream << "Adres_Priem = " << "0x89" << "\n";
        stream << "Baund = " << "9600" << "\n";
        stream << "Parity = " << "NONE" << "\n";
        stream << "Stop_Bits = " << "1" << "\n";

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

void UpWidg::set_time(QString time)
{
    timer->start(1000);
    lbl_time->setText(time);
}

void UpWidg::addTime()
{
    QString timeStr = lbl_time->text().trimmed();

    QLocale locale(QLocale::C); // Используем стандартную локаль (точка как разделитель)
    QDateTime dt = locale.toDateTime(timeStr, "dd.MM.yyyy HH:mm:ss");

    if (!dt.isValid()) {
        qDebug() << "Ошибка парсинга! Устанавливаю текущее время.";
        dt = QDateTime::currentDateTime();
    }
    dt = dt.addSecs(1);
    QString newTimeStr = dt.toString("dd.MM.yyyy HH:mm:ss");
    lbl_time->setText(newTimeStr);
}


