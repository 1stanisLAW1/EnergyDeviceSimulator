#include "logiwrite.h"
#include "qdatetime.h"
#include "qevent.h"

#include <QCoreApplication>
#include <QFile>

LogiWrite::LogiWrite(QObject *parent)
    : QObject{parent}
{}

void LogiWrite::write_request()
{
    QString executablePath = QCoreApplication::applicationDirPath();
    QString filePath = executablePath + "/../conf/log_requests.txt";
    QFile file(filePath);

    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);

        for(int i = 0; i<list.size();i++){
            stream<<list.at(i)+"\n";
        }
        // Проверка на ошибки записи
        if (stream.status() != QTextStream::Ok) {
            qDebug() << "Ошибка записи данных: " << filePath << stream.status();
        }

        file.close();

        // Проверка на ошибки закрытия файла
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
    QString filePath = executablePath + "/../conf/log_answer.txt";
    QFile file(filePath);

    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);

        for(int i = 0; i<list_answer.size();i++){
            stream<<list_answer.at(i)+"\n";
        }
        // Проверка на ошибки записи
        if (stream.status() != QTextStream::Ok) {
            qDebug() << "Ошибка записи данных: " << filePath << stream.status();
        }

        file.close();

        // Проверка на ошибки закрытия файла
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
    QString filePath = executablePath + "/../conf/log_requests.txt";
    QString filePath_2 = executablePath + "/../conf/log_answer.txt";
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
