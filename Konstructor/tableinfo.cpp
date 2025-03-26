#include "tableinfo.h"

#include <QTableWidget>

tableInfo::tableInfo(Moduls *)
{
    QTableWidget*table = new QTableWidget;
    table->setRowCount(5);
    table->setColumnCount(2);
}
