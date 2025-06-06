#ifndef LOGIWRITE_H
#define LOGIWRITE_H

#include <QObject>

class LogiWrite : public QObject
{
    Q_OBJECT
public:
    explicit LogiWrite(QObject *parent = nullptr);
    void write_request();
    void write_answer(QList<QString>list_answer);
    void clear_logs();
    void appendList(QString,bool);
    void saveConfig(QList<QWidget*>list);
    void writeConfig(QStringList);

private:
    QList<QString>list;
    QList<QString>list_answer;
    QStringList names;
};

#endif // LOGIWRITE_H
