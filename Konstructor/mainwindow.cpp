#include "mainwindow.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qobjectdefs.h"
#include "ui_mainwindow.h"
#include "iniloader.h"
#include "graphicmodul.h"
#include <QPushButton>
#include <QMessageBox>
#include <QGraphicsView>
#include <QTableWidget>
#include <QFont>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout*  mainLt = new QHBoxLayout;
    centralWidget->setLayout(mainLt);
    setCentralWidget(centralWidget);

    scene = new GraphicsScene(this);
    modulList = new QWidget(this);
    QVBoxLayout* lt = new QVBoxLayout();
    modulList->setLayout(lt);
    table = new QTableWidget(this);
    table->setColumnCount(2);
    table->setColumnWidth(0,180);
    table->setColumnWidth(1,120);
    table->setHorizontalHeaderLabels(QStringList()<<"Name"<<"Length");

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addWidget(modulList);
    leftLayout->addWidget(table);
    mainLt->addLayout(leftLayout);

   QString executablePath = QCoreApplication::applicationDirPath();

    dirPath = executablePath + "/../../../DB";

    QPushButton *btnDialog= new QPushButton();
    btnDialog->setText("Download");
    QPushButton *btnSave= new QPushButton();
    btnSave->setText("Save");
    QHBoxLayout *hbxL = new QHBoxLayout();
    hbxL->addWidget(btnSave);
    hbxL->addWidget(btnDialog);

   connect(btnDialog, &QPushButton::clicked, this, &MainWindow::onBtnClickedDialog);
   connect(btnSave,&QPushButton::clicked,this,&MainWindow::onBtnClickedSave);

    mIniLoader = new Iniloader(this);

    connect(mIniLoader, &Iniloader::sendMessage, this, &MainWindow::showMessage);
    connect(&mTelSystem, &Telesystem::txtEd, this, &MainWindow::showMessage);
    connect(&mTelSystem, &Telesystem::updated, this, &MainWindow::upDateTable);

    scene->setSceneRect(0,-25,200,10000);
    QGraphicsView *view = new QGraphicsView(scene);
    QVBoxLayout *rightLt = new QVBoxLayout;
    txtEdit = new QTextEdit(this);
    txtEdit->isReadOnly();
    rightLt->addWidget(view);
    rightLt->addLayout(hbxL);
    rightLt->addWidget(txtEdit);

    mainLt->addLayout(rightLt);

    mTelSystem.setScene(scene);
    connect(&mTelSystem,SIGNAL(updated()),this,SLOT(upDateTable()));
    getModules();
    showModules();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getModules()
{
    QString executablePath = QCoreApplication::applicationDirPath();
    QString path=executablePath+"/../../../ini_files";
    QList<Moduls> moduls = mIniLoader->createModuls(path);
    mModules.append(moduls);
}

void MainWindow::showModules()
{
    QList<Moduls>modul;

    for(int i = 0;i<mModules.size();i++){
        QPushButton *btn = new QPushButton(mModules.at(i).name(),this);
        btn->setProperty("index",QVariant(i));
        connect(btn,&QPushButton::clicked,this,&MainWindow::onBtnClicked);

        modulList->layout()->addWidget(btn);
    }
}

void MainWindow::showMessage(int e,QString str)
{
    if(e==0){
        txtEdit->append("Error: "+str);
    }else{
        txtEdit->append(str);
    }
}

void MainWindow::upDateTable(){
    clearTable();

    int rowCount = 0;
    QList <Moduls*>telesystemModuls = mTelSystem.getModuls();
    for(int i = 0;i<telesystemModuls.size();i++){
        rowCount++;
        rowCount+=telesystemModuls.at(i)->pointsCount();
    }
    table->setRowCount(rowCount);
    int row = 0;
    int fullen = 0;
    for(int a = 0;a<telesystemModuls.size();a++){
        QTableWidgetItem*item = new QTableWidgetItem(telesystemModuls.at(a)->name());
        item->setFont(QFont("Times",10,QFont::Bold | QFont::Black));
        table->setItem(row,0,item);
        item = new QTableWidgetItem(QString::number(fullen));
        table->setItem(row,1,item);
        row++;
        QList<Point>points = telesystemModuls.at(a)->getPoints();
        for(int i = 0;i<points.size();i++){
            QString pointName = points.at(i).name();
            item = new QTableWidgetItem(pointName);
            table->setItem(row,0,item);
            int l = points.at(i).lent()+fullen;
            item = new QTableWidgetItem(QString::number(l));
            table->setItem(row,1,item);
            qDebug()<<item->data(Qt::DisplayPropertyRole)<<l<<pointName;
            row++;
        }
        fullen+=telesystemModuls.at(a)->len();
    }
}

void MainWindow::onBtnClicked()
{
    QObject* ob = sender();
    QPushButton* btn = qobject_cast<QPushButton*>(ob);
    if (!btn) { return; }
    QVariant v=btn->property("index");
    int indx = v.toInt();
    if (indx >= mModules.size()) { return; }
    Moduls m = mModules[indx];
    mTelSystem.appendModul(m);
    return;
}

void MainWindow::onBtnClickedDialog()
{
    qDebug()<<"Dialog";
    QDir dir(dirPath);
    //Фильтр
    QStringList filters;
    filters << "*.txt";
    QStringList files = dir.entryList(filters, QDir::Files);

    //получаем список всех файлов в директории
    QStringList files_2 = dir.entryList(QDir::Files);

    // Создаем список пар (время изменения, имя файла)
    QList<QPair<QDateTime, QString>> fileInfoList;
    foreach (QString file, files) {
        QFileInfo fileInfo(dir.absoluteFilePath(file));
        fileInfoList.append(qMakePair(fileInfo.lastModified(), file));
    }

    // Сортируем список по времени изменения (от новых к старым)
    std::sort(fileInfoList.begin(), fileInfoList.end(), [](const QPair<QDateTime, QString>& a, const QPair<QDateTime, QString>& b) {
        return a.first > b.first;
    });

    QDialog* dialog = new QDialog();
    QScrollArea* scrollArea = new QScrollArea(dialog);
    scrollArea->setWidgetResizable(true);
    QWidget *widget = new QWidget();
    dialog->resize(400,200);
    QVBoxLayout* lt = new QVBoxLayout(widget);

    foreach (const auto& filePair, fileInfoList) {
        QFileInfo fileInfo(filePair.second);
        QString baseName = fileInfo.baseName();
        QPushButton* btn = new QPushButton(baseName);
        lt->addWidget(btn);

        connect(btn, &QPushButton::clicked, this, [this, btn]() {
            loadFromTxtFile(btn->text());
        });
    }
    QDialogButtonBox * btnBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,dialog);

    connect(btnBox,&QDialogButtonBox::accepted,dialog,&QDialog::accept);
    connect(btnBox,&QDialogButtonBox::rejected,dialog,&QDialog::reject);


    widget->setLayout(lt);
    scrollArea->setWidget(widget);
    QVBoxLayout* dialogLayout = new QVBoxLayout(dialog);
    dialogLayout->addWidget(scrollArea);
    dialogLayout->addWidget(btnBox);
    dialog->setLayout(dialogLayout);


    if(dialog->exec()!=QDialog::Accepted){return;}
}

void MainWindow::onBtnClickedSave()
{
    qDebug() << "Save";

    QLabel *lbl = new QLabel("Введите имя файла");
    QLineEdit *LD = new QLineEdit();
    QDialog* dialog = new QDialog();
    QPushButton *btn = new QPushButton("Save");

    dialog->resize(300, 100);

    QVBoxLayout* lt = new QVBoxLayout();
    lt->addWidget(lbl);
    lt->addWidget(LD);
    lt->addWidget(btn);
    dialog->setLayout(lt);

    connect(btn, &QPushButton::clicked, this, [this, dialog, LD]() {
        if (LD != nullptr && !LD->text().isEmpty()) {
            if (streamText(LD->text())) {
                dialog->accept();
            }
        }
    });
    dialog->exec();
}

bool MainWindow::streamText(QString str)
{
    QList <Moduls*>telesystemModuls = mTelSystem.getModuls();
    int fullen = 0;
    QFile file(dirPath+"/"+str+".txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream stream(&file);
        for(int a = 0;a<telesystemModuls.size();a++){
            stream <<"Name-"<<telesystemModuls.at(a)->name()<<" Total length- "<<fullen<<" Length- "<<telesystemModuls.at(a)->len()<<Qt::endl;
            QList<Point>points = telesystemModuls.at(a)->getPoints();
            for(int i = 0;i<points.size();i++){
                QString pointName = points.at(i).name();
                int l = points.at(i).lent()+fullen;
                stream <<"Name-"<<pointName<<" Total length- "<<l<<" Length- "<< points.at(i).lent()<<Qt::endl;
            }
            fullen+=telesystemModuls.at(a)->len();
        }
        if(stream.status() != QTextStream::Ok){
            QChar character = static_cast<QChar>(stream.status());
            showMessage(0,"Ошибка записи данных: " + dirPath +" " + character);
            file.close();
            return false;
        }
        if(file.error() != QFile::NoError){
            showMessage(0,"Ошибка закрытия файла");
            return false;
        }
    }
    showMessage(1,"Данные успешно сохранены!");
    return true;
}

void MainWindow::loadFromTxtFile(QString str)
{
    mTelSystem.clearScene();
    QString pathF = dirPath + "/" + str + ".txt";

    QFile file(pathF);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        showMessage(0, "Не удалось открыть файл: " + pathF);
        return;
    }

    showMessage(1, "Файл успешно открыт!");

    QTextStream read(&file);
    while (!read.atEnd()) {
        QString line = read.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(" ");
        if (parts.size() < 6) continue;

        QString name;
        if (parts[0].startsWith("Name-")) {
            name = parts[0].mid(5);
            for (int i = 1; i < parts.size(); i++) {
                if (parts[i].startsWith("Total")) break;
                name += " " + parts[i];
            }
        } else {
            continue;
        }

        int totalLength = -1;
        int moduleLength = -1;
        for (int i = 0; i < parts.size() - 1; i++) {
            if (parts[i] == "Total" && parts[i+1] == "length-") {
                if (i+2 < parts.size()) {
                    totalLength = parts[i+2].toInt();
                }
            }
            else if (parts[i] == "Length-") {
                if (i+1 < parts.size()) {
                    moduleLength = parts[i+1].toInt();
                }
            }
        }

        if (totalLength == -1 || moduleLength == -1) continue;

        for (Moduls& mod : mModules) {
            if (mod.name() == name) {
                Moduls newModule = mod;
                newModule.setCurrentLength(moduleLength);
                ModulDowload.append(newModule);
                break;
            }
        }
    }
    file.close();
    load();
}

void MainWindow::load()
{
    for (const Moduls& mod : ModulDowload) {
        mTelSystem.appendModul(mod);
    }
}

void MainWindow::clearTable()
{
    while (table->rowCount() > 0)
    {
        table->removeRow(0);
    }
}
