#ifndef SENDINGCOMMANDS_H
#define SENDINGCOMMANDS_H

#include <QObject>

class SendingCommands : public QObject
{
    Q_OBJECT
public:
    explicit SendingCommands(QObject *parent = nullptr);
    QWidget* compWidget(QWidget*widget);
    QWidget* compWidget_2(QWidget*widget);
    QWidget* compWidget_3(QWidget*widget);
    QWidget* compWidget_4(QWidget*widget);
    QWidget* compWidget_5(QWidget*widget);
    QWidget* compWidget_6(QWidget*widget);

signals:
private:
    //QVector<int>value;
    QVector<QString>lbl_text;
    QVector<QString>lbl_text_2;
    QVector<QString>lbl_text_3;
    QVector<QString>lbl_text_4;
};

#endif // SENDINGCOMMANDS_H
