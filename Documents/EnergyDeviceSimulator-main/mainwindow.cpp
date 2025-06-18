#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "upwidg.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QLineEdit>
#include <QScrollArea>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //создание всех элементов

    upW = new UpWidg();
    sComm = new SendingCommands();


    //this->setFixedSize(640,480);
    tabWidget = new QTabWidget();
    tabWidget = sComm->compTabWidget(tabWidget);

    QVBoxLayout*vLayoutMain = new QVBoxLayout();
    QWidget* upWidget = upW->upWidget();
    upWidget->setFixedHeight(150);

    vLayoutMain->addWidget(upWidget);
    vLayoutMain->addWidget(tabWidget);

    ui->centralwidget->setLayout(vLayoutMain);


    connect(upW, &UpWidg::updateRequested, this, &MainWindow::appendList);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete upW;
    delete vLayout;
    delete lineEdit;
    delete lblLeftPanel_2;
    delete sComm;
    delete tabWidget;
}

void MainWindow::appendList()
{
    QList<QWidget*>widget_list;
    for(int i =0;i<6;i++){
        widget_list.append(tabWidget->widget(i));
    }
    upW->appendList(widget_list);
}
