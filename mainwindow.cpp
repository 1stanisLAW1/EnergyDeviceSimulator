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
    QWidget* tabWidge_1 = new QWidget();
    QWidget* tabWidge_2 = new QWidget();
    QWidget* tabWidge_3 = new QWidget();
    QWidget* tabWidge_4 = new QWidget();
    QWidget* tabWidge_5 = new QWidget();
    QWidget* tabWidge_6 = new QWidget();


    tabWidget->addTab(sComm->compWidget(tabWidge_1),"0х01");
    tabWidget->addTab(sComm->compWidget_2(tabWidge_2),"0х02,0х03,0х04,0х05");
    tabWidget->addTab(sComm->compWidget_3(tabWidge_3),"0х06");
    tabWidget->addTab(sComm->compWidget_4(tabWidge_4),"0х07, 0х08 ,0х0A");
    tabWidget->addTab(sComm->compWidget_5(tabWidge_5),"0х0B ,0х0D");
    tabWidget->addTab(sComm->compWidget_6(tabWidge_6),"0х0C,0х0E,0х18,0хFC");

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
    for(int i =0;i<6;i++){
        upW->appendList(tabWidget->widget(i));
    }
}
