#ifndef UPWIDGETSET_H
#define UPWIDGETSET_H

#include "logic.h"
#include "qcombobox.h"
#include "qwidget.h"
class upWidgetSet
{
public:
    upWidgetSet();
    QWidget* upWidget();
    void setItem_ComboBox();
    void updateValues();
private:
    QWidget*widget;
    Logic*log;
    QComboBox* comboBox_COM;
    QComboBox* comboBox_Bould;
    QComboBox* comboBox_Parity;
    QComboBox* comboBox_StopBits;
};

#endif // UPWIDGETSET_H
