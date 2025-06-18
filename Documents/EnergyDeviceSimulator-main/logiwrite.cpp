#include "logiwrite.h"
#include "qdatetime.h"
#include "qevent.h"
#include "qlineedit.h"

#include <QCoreApplication>
#include <QFile>

LogiWrite::LogiWrite(QObject *parent)
    : QObject{parent}
{
    for(int i = 0;i<16;i++){
        names+=QString("DT_%1 = ").arg(i);
    }
    for(int i = 0;i<16;i++){
        names+=QString("NC_%1 = ").arg(i);
    }
    names +={"DTA = ","DTB = ","DTC = ","DTD = ","DTE = ",
"DTF = ","DTG = ","DTH = ","DTUP = ","DTWR = ",
"DTDN = ","DTLF = ","TOP = ","BOT = ","W = ","DEN = ","N = "};

    for(int i = 0;i<16;i++){
        names+=QString("DR_%1 = ").arg(i);
    }
    for(int i = 0;i<16;i++){
        names+=QString("RD_%1 = ").arg(i);
    }
    names +={"ZNT = ","CRV = ","IGK_1 = ","IGK_2 = ","IGK_3 = ",
    "BA = ","CS = ","PE = ","DAC = ","CA = "};
    for(int i = 0;i<16;i++){
        names+=QString("RA_%1 = ").arg(i);
    }
    names +={"DMIN = ","DMAX = ","PRS_1 = ","PRS_2 = ","TMP_1 = ",
             "TMP_2 = ","NEAR = ","NFAR = ","NUM = ","RSRV = ","id = "};
}

void LogiWrite::write_request()
{
    QString executablePath = QCoreApplication::applicationDirPath();
    QString filePath = executablePath + "/conf/log_requests.txt";
    QFile file(filePath);

    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);

        for(int i = 0; i<list.size();i++){
            stream<<list.at(i)+"\n";
        }

        if (stream.status() != QTextStream::Ok) {
            qDebug() << "Ошибка записи данных: " << filePath << stream.status();
        }

        file.close();

        if (file.error() != QFile::NoError) {
            qDebug() << "Ошибка закрытия файла";
        }
    } else {
        qDebug() << "Не удалось открыть файл для записи";
    }
}

void LogiWrite::write_answer(QList<QString>list_answer)
{
    QString executablePath = QCoreApplication::applicationDirPath();
    QString filePath = executablePath + "/conf/log_answer.txt";
    QFile file(filePath);

    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);

        for(int i = 0; i<list_answer.size();i++){
            stream<<list_answer.at(i)+"\n";
        }
        if (stream.status() != QTextStream::Ok) {
            qDebug() << "Ошибка записи данных: " << filePath << stream.status();
        }

        file.close();

        if (file.error() != QFile::NoError) {
            qDebug() << "Ошибка закрытия файла";
        }
    } else {
        qDebug() << "Не удалось открыть файл для записи";
    }
}

void LogiWrite::clear_logs()
{
    QString executablePath = QCoreApplication::applicationDirPath();
    QString filePath = executablePath + "/conf/log_requests.txt";
    QString filePath_2 = executablePath + "/conf/log_answer.txt";
    QFile file(filePath);
    QFile file_2(filePath_2);
    list_answer.clear();
    list.clear();

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << "";
        file.close();
        qDebug() << "Лог-файл очищен";
    } else {
        qDebug() << "Ошибка очистки лог-файла:" << file.errorString();
    }
    file.close();
    if (file_2.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QTextStream stream_2(&file_2);
        stream_2 << "";
        file_2.close();
        qDebug() << "Лог-файл очищен";
    } else {
        qDebug() << "Ошибка очистки лог-файла:" << file.errorString();
    }
    file_2.close();
}

void LogiWrite::appendList(QString str, bool ok)
{
    QString full_str = QTime::currentTime().toString("HH:mm:ss:zzz")+" - "+str+" - "+QString::number(ok);
    list.append(full_str);
}

void LogiWrite::saveConfig(QList<QWidget *> list)
{

    QStringList list_str;

    QList<QLineEdit*>list_line;
    for (QWidget *widget : list) {
        list_line += widget->findChildren<QLineEdit*>();
    }
    for(int i = 0;i<list_line.size();i++){
        list_str.append(list_line.at(i)->text());
    }
    writeConfig(list_str);
}

void LogiWrite::writeConfig(QStringList list_str)
{

    QString executablePath = QCoreApplication::applicationDirPath();
    QString filePath = executablePath + "/conf/data_config.txt";
    QFile file(filePath);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream<<"\n";

        for(int i = 0;i<list_str.size();i++){
            stream<<names.at(i)<<list_str.at(i)<<"\n";
        }

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
