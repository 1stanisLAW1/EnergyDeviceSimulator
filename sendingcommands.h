#ifndef SENDINGCOMMANDS_H
#define SENDINGCOMMANDS_H

#include <QObject>
#include <QTabWidget>

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
    QTabWidget* compTabWidget(QTabWidget*tab_widget);
    void load_values();

signals:
private:
    QVector<QString>values;
    QVector<QString>lbl_text;
    QVector<QString>lbl_text_2;
    QVector<QString>lbl_text_3;
    QVector<QString>lbl_text_4;
    QVector<QString>Values;
    QVector<QString>lbl_text_all;

};

#endif // SENDINGCOMMANDS_H
