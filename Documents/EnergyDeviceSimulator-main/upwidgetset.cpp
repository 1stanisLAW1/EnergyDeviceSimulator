#include "upwidgetset.h"
#include "qdialog.h"
#include "qlabel.h"
#include "qlayout.h"

#include <QComboBox>
#include <QPushButton>

upWidgetSet::upWidgetSet() {
    widget = new QWidget();
    log = new Logic();
}

QWidget *upWidgetSet::upWidget()
{
    QLabel* lblCOM = new QLabel("COM port:");
    QLabel* lblBould = new QLabel("Bould rate:");
    QLabel* lblParity = new QLabel("Parity");
    QLabel* lblStopBits = new QLabel("Stop bits:");

    comboBox_COM = new QComboBox();
    comboBox_Bould = new QComboBox();
    comboBox_Parity = new QComboBox();
    comboBox_StopBits = new QComboBox();

    QPushButton* btnStart = new QPushButton("Start");
    QPushButton* btnUpdate = new QPushButton("Update values");

    QLabel* lbl = new QLabel("Request Packs");
    QLabel* lblValue = new QLabel("0");


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
    hLayout_Down->addWidget(lbl);
    hLayout_Down->addWidget(lblValue);

    widget->setLayout(vLayout);

    setItem_ComboBox();

    connect(btnUpdate, &QPushButton::clicked, this, &upWidgetSet::updateValues);

    return widget;
}

void upWidgetSet::setItem_ComboBox()
{
    comboBox_COM->clear();
    comboBox_Bould->clear();
    comboBox_Parity->clear();
    comboBox_StopBits->clear();

    QVector<QString>ports_Name = log->showPortsName();

    for(int i =0; i<ports_Name.size();i++){
        comboBox_COM->addItem(ports_Name.at(i));
    }

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

void upWidgetSet::updateValues()
{
    comboBox_COM->clear();
    QVector<QString>ports_Name = log->showPortsName();

    for(int i =0; i<ports_Name.size();i++){
        comboBox_COM->addItem(ports_Name.at(i));
    }
}
