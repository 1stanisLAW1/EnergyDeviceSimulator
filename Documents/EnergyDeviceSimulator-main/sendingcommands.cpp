#include "sendingcommands.h"
#include "qboxlayout.h"
#include "qevent.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qwidget.h"
#include "QGridLayout"
#include "QFile"

SendingCommands::SendingCommands(QObject *parent)
    : QObject{parent}
{
    lbl_text = {"DTA","DTB","DTC","DTD","DTE","DTF",
                "DTG","DTH","DTUP","DTWR","DTDN","DTLF",
                "TOP","BOT","W","DEN","N"};

    lbl_text_2 = {"ZNT","CRV","IGK_1","IGK_2",
                  "IGK_3","BA","CS","PE","DAC","CA"};

    for(int i =0;i<16;i++){lbl_text_3.append("RA_"+QString::number(i));} lbl_text_3.append("DMIN"); lbl_text_3.append("DMAX");

    lbl_text_4 ={"PRS_1","PRS_2","TMP_1","TMP_2",
                  "NEAR","NFAR","NUM","RSRV","id"};

    for(int i =0;i<16;i++){lbl_text_all.append("DT_"+QString::number(i));}
     for(int i =0;i<16;i++){lbl_text_all.append("NC_"+QString::number(i));}


    lbl_text_all += lbl_text;
     for(int i =0;i<16;i++){lbl_text_all.append("DR_"+QString::number(i));}
     for(int i =0;i<16;i++){lbl_text_all.append("RD_"+QString::number(i));}

    lbl_text_all += lbl_text_2;
   for(int i =0;i<16;i++){lbl_text_all.append("RA_"+QString::number(i));} lbl_text_all.append("DMIN"); lbl_text_all.append("DMAX");
    lbl_text_all += lbl_text_4;

    load_values();
}

QWidget *SendingCommands::compWidget(QWidget *widget)
{
    QHBoxLayout*layout = new QHBoxLayout(widget);
    QGridLayout* grid_Layout = new QGridLayout();
    QGridLayout* grid_Layout_2 = new QGridLayout();

    for(int i = 0;i<16;i++){

            QHBoxLayout* h_Layout = new QHBoxLayout();
            QLabel* lbl = new QLabel("DT_"+QString::number(i));
            QLineEdit* line_Edit = new QLineEdit(values.at(i));

            h_Layout->addWidget(lbl);
            h_Layout->addWidget(line_Edit);

            int row = i/2;
            int col = i%2;

            grid_Layout->addLayout(h_Layout, row, col);
    }

    for(int i = 0;i<16;i++){

        QHBoxLayout* h_Layout = new QHBoxLayout();
        QLabel* lbl = new QLabel("NC_"+QString::number(i));
        QLineEdit* line_Edit = new QLineEdit(values.at(i+16));

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
        QLineEdit* line_Edit = new QLineEdit(values.at(i+32));
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
        QLineEdit* line_Edit = new QLineEdit(values.at(i+49));

        h_Layout->addWidget(lbl);
        h_Layout->addWidget(line_Edit);

        int row = i/2;
        int col = i%2;

        grid_Layout->addLayout(h_Layout, row, col);
    }

    for(int i = 0;i<16;i++){

        QHBoxLayout* h_Layout = new QHBoxLayout();
        QLabel* lbl = new QLabel("RD_"+QString::number(i));
        QLineEdit* line_Edit = new QLineEdit(values.at(i+65));

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
        QLineEdit* line_Edit = new QLineEdit(values.at(i+81));

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
        QLineEdit* line_Edit = new QLineEdit(values.at(i+91));

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
        QLineEdit* line_Edit = new QLineEdit(values.at(i+109));

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

QTabWidget *SendingCommands::compTabWidget(QTabWidget *tab_widget)
{
    QWidget* tabWidge_1 = new QWidget();
    QWidget* tabWidge_2 = new QWidget();
    QWidget* tabWidge_3 = new QWidget();
    QWidget* tabWidge_4 = new QWidget();
    QWidget* tabWidge_5 = new QWidget();
    QWidget* tabWidge_6 = new QWidget();

    tab_widget->addTab(compWidget(tabWidge_1),"плотность и водород");
    tab_widget->addTab(compWidget_2(tabWidge_2),"плотность и интегральный водород");
    tab_widget->addTab(compWidget_3(tabWidge_3),"зазоры и поправки плотности");
    tab_widget->addTab(compWidget_4(tabWidge_4),"Z угол,ГК,фотоэлектрический эффект");
    tab_widget->addTab(compWidget_5(tabWidge_5),"R и D по акустике и ГГК");
    tab_widget->addTab(compWidget_6(tabWidge_6),"плотность и t ,БЗ и МЗ ННК,идентификатор прибора");

    return tab_widget;
}

void SendingCommands::load_values()
{
    QString executablePath = QCoreApplication::applicationDirPath();
    QString filePath = executablePath + "/conf/data_config.txt";

    QFile file(filePath);
    if (!file.exists() || file.size() == 0) {
        file.close();
        return;
    }

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        QStringList result;
        bool found = false;

        while (!stream.atEnd()) {
            QString line = stream.readLine().trimmed();
            if (line.isEmpty()) continue;

            if(lbl_text_all.size() == 0) {
                file.close();
                return;
            }

            for(int i = 0; i < lbl_text_all.size(); i++) {
                QString pattern = QString("%1 = ").arg(lbl_text_all.at(i));
                if (line.startsWith(pattern)) {
                    QString value = line.mid(pattern.length()).trimmed();
                    values.append(value);
                    if (!value.isEmpty()) {
                        result.append(value);
                        found = true;
                    }
                }
            }
        }

        file.close();

        if (!found) {
            qDebug() << "Параметры не найдены в файле";
        }

        return;
    } else {
        qDebug() << "Error: Не удалось открыть файл для чтения";
        return;
    }
}

