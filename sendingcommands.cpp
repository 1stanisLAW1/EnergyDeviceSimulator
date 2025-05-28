#include "sendingcommands.h"
#include "qboxlayout.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qwidget.h"
#include "QGridLayout"

SendingCommands::SendingCommands(QObject *parent)
    : QObject{parent}
{
    //value = {1,10,3,100,4,0,100,1,1,1,10,3,20,4,30,2,0,0};
    lbl_text = {"DTA","DTB","DTC","DTD","DTE","DTF",
                "DTG","DTH","DTUP","DTWR","DTDN","DTLF",
                "TOP","BOT","W","DEN","N"};

    lbl_text_2 = {"ZNT","CRV","IGK_1","IGK_2",
                  "IGK_3","BA","CS","PE","DAC","CA"};

    for(int i =0;i<16;i++){lbl_text_3.append("RA_"+QString::number(i));} lbl_text_3.append("DMIN"); lbl_text_3.append("DMAX");

    lbl_text_4 ={"PRS_1","PRS_2","TMP_1","TMP_2",
                  "NEAR","NFAR","id_1",
                "id_2","id_3","id_4","NUM","RSRV"};
}

QWidget *SendingCommands::compWidget(QWidget *widget)
{
    QHBoxLayout*layout = new QHBoxLayout(widget);
    QGridLayout* grid_Layout = new QGridLayout();
    QGridLayout* grid_Layout_2 = new QGridLayout();

    for(int i = 0;i<16;i++){

            QHBoxLayout* h_Layout = new QHBoxLayout();
            QLabel* lbl = new QLabel("DT_"+QString::number(i));
            QLineEdit* line_Edit = new QLineEdit(QString::number(i));

            h_Layout->addWidget(lbl);
            h_Layout->addWidget(line_Edit);

            int row = i/2;
            int col = i%2;

            grid_Layout->addLayout(h_Layout, row, col);
    }

    for(int i = 0;i<16;i++){

        QHBoxLayout* h_Layout = new QHBoxLayout();
        QLabel* lbl = new QLabel("NC_"+QString::number(i));
        QLineEdit* line_Edit = new QLineEdit(QString::number(i));

        h_Layout->addWidget(lbl);
        h_Layout->addWidget(line_Edit);

        int row = i / 2;
        int col = i % 2;

        grid_Layout_2->addLayout(h_Layout, row, col);
    }

    layout->addLayout(grid_Layout);
    layout->addLayout(grid_Layout_2);

    widget->setLayout(layout);

    return widget;
}

QWidget *SendingCommands::compWidget_2(QWidget *widget)
{
    QGridLayout* grid_Layout = new QGridLayout(widget);

    for(int i = 0;i<lbl_text.size();i++){

        QHBoxLayout* h_Layout = new QHBoxLayout();
        QLabel* lbl = new QLabel(lbl_text.at(i));
        QLineEdit* line_Edit = new QLineEdit(QString::number(i));
        lbl->setFixedWidth(50);
        line_Edit->setFixedWidth(200);

        h_Layout->addWidget(lbl);
        h_Layout->addWidget(line_Edit);

        int row = i/3;
        int col = i%3;

        grid_Layout->addLayout(h_Layout, row, col);
    }


    widget->setLayout(grid_Layout);
    return widget;
}

QWidget *SendingCommands::compWidget_3(QWidget *widget)
{
    QHBoxLayout*layout = new QHBoxLayout(widget);
    QGridLayout* grid_Layout = new QGridLayout();
    QGridLayout* grid_Layout_2 = new QGridLayout();

    for(int i = 0;i<16;i++){

        QHBoxLayout* h_Layout = new QHBoxLayout();
        QLabel* lbl = new QLabel("DR_"+QString::number(i));
        QLineEdit* line_Edit = new QLineEdit(QString::number(i));

        h_Layout->addWidget(lbl);
        h_Layout->addWidget(line_Edit);

        int row = i/2;
        int col = i%2;

        grid_Layout->addLayout(h_Layout, row, col);
    }

    for(int i = 0;i<16;i++){

        QHBoxLayout* h_Layout = new QHBoxLayout();
        QLabel* lbl = new QLabel("RD_"+QString::number(i));
        QLineEdit* line_Edit = new QLineEdit(QString::number(i));

        h_Layout->addWidget(lbl);
        h_Layout->addWidget(line_Edit);

        int row = i / 2;
        int col = i % 2;

        grid_Layout_2->addLayout(h_Layout, row, col);
    }

    layout->addLayout(grid_Layout);
    layout->addLayout(grid_Layout_2);

    widget->setLayout(layout);

    return widget;
}

QWidget *SendingCommands::compWidget_4(QWidget *widget)
{
    QGridLayout* grid_Layout = new QGridLayout(widget);

    for(int i = 0;i<lbl_text_2.size();i++){

        QHBoxLayout* h_Layout = new QHBoxLayout();
        QLabel* lbl = new QLabel(lbl_text_2.at(i));
        QLineEdit* line_Edit = new QLineEdit(QString::number(i));

        h_Layout->addWidget(lbl);
        h_Layout->addWidget(line_Edit);

        int row = i/2;
        int col = i%2;

        grid_Layout->addLayout(h_Layout, row, col);
    }


    widget->setLayout(grid_Layout);

    return widget;
}

QWidget *SendingCommands::compWidget_5(QWidget *widget)
{
    QHBoxLayout*layout = new QHBoxLayout(widget);
    QGridLayout* grid_Layout = new QGridLayout();

    for(int i = 0;i<lbl_text_3.size();i++){

        QHBoxLayout* h_Layout = new QHBoxLayout();
        QLabel* lbl = new QLabel(lbl_text_3.at(i));
        QLineEdit* line_Edit = new QLineEdit(QString::number(i));

        h_Layout->addWidget(lbl);
        h_Layout->addWidget(line_Edit);

        int row = i/3;
        int col = i%3;

        grid_Layout->addLayout(h_Layout, row, col);
    }

    layout->addLayout(grid_Layout);

    widget->setLayout(layout);

    return widget;
}

QWidget *SendingCommands::compWidget_6(QWidget *widget)
{
    QHBoxLayout*layout = new QHBoxLayout(widget);
    QGridLayout* grid_Layout = new QGridLayout();

    for(int i = 0;i<lbl_text_4.size();i++){

        QHBoxLayout* h_Layout = new QHBoxLayout();
        QLabel* lbl = new QLabel(lbl_text_4.at(i));
        QLineEdit* line_Edit = new QLineEdit(QString::number(i));

        h_Layout->addWidget(lbl);
        h_Layout->addWidget(line_Edit);

        int row = i/3;
        int col = i%3;

        grid_Layout->addLayout(h_Layout, row, col);
    }

    layout->addLayout(grid_Layout);

    widget->setLayout(layout);

    return widget;
}

