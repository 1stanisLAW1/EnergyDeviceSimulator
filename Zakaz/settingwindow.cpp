#include "settingwindow.h"
#include "mainwindow.h"
#include "ui_settingwindow.h"
#include "menu.h"

settingWindow::settingWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::settingWindow)
{
    ui->setupUi(this);
    DB_Connect = QSqlDatabase::addDatabase("QSQLITE");
    DB_Connect.setDatabaseName(QCoreApplication::applicationDirPath()+"/../DB/Zak.db");
    styleSheet =         "QMessageBox {"
                 "   background-color:rgb(0, 0, 0);"
                 "}"

                 "QMessageBox QLabel {"
                 "   color:rgb(255, 255, 255);"
                 "   font: bold 12pt;"
                 "}";
    if(!DB_Connect.open()){
        msgBox = new QMessageBox();
        msgBox->setWindowTitle("Ошибка");
        msgBox->setText(DB_Connect.lastError().text());
        msgBox->setIcon(QMessageBox::Critical);
        msgBox->setStyleSheet(styleSheet);
        msgBox->exec();
    }
    this->setWindowTitle("Settings");
}

settingWindow::~settingWindow()
{
    DB_Connect.close();
    delete ui;
}

void settingWindow::on_pushButton_clicked()
{
    MenU m;
    this->hide();
    m.show();
    m.exec();
}


void settingWindow::on_pushButton_2_clicked()
{
    QString directoryPath = QFileDialog::getExistingDirectory(this, tr("Select Directory"), "", QFileDialog::ShowDirsOnly);
    if (directoryPath.isEmpty()) {
        return;
    }

    QDir dir(directoryPath);
    QStringList filters;
    filters << "*.png *.jpg *.ico *.gif";
    dir.setNameFilters(filters);
    QStringList image = dir.entryList(QDir::Files);
    QString filePathImage = QFileDialog::getOpenFileName(this, tr("Select image"), directoryPath, tr("Image Files (*.png *.jpg *.ico *.gif)"));
    QFileInfo fileInfo(filePathImage);

    QString executablePath = QCoreApplication::applicationDirPath();
    QString filePath = executablePath + "/../TxT/Log.txt";

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        //ui->labe->setText(stream.readAll());
        file.close();

        msgBox = new QMessageBox();
        msgBox->setWindowTitle("Ошибка");
        msgBox->setText("Не удалось открыть файл для записи");
        msgBox->setIcon(QMessageBox::Critical);
        msgBox->setStyleSheet(styleSheet);
        msgBox->exec();
    }
    QTextStream stream(&file);
    QString log = stream.readAll();
    if (log.isEmpty()) {
        msgBox = new QMessageBox();
        msgBox->setWindowTitle("Ошибка");
        msgBox->setText("Файл с именем пуст: " + filePath);
        msgBox->setIcon(QMessageBox::Critical);
        msgBox->setStyleSheet(styleSheet);
        msgBox->exec();
    }

    QSqlDatabase db = QSqlDatabase();
    QSqlQuery query(DB_Connect);
    query.prepare("UPDATE log SET image_path = :image_path WHERE LogiN = :logi");
    query.bindValue(":image_path", filePathImage);
    query.bindValue(":logi", log);
    if (!query.exec()) {
        msgBox = new QMessageBox();
        msgBox->setWindowTitle("Ошибка");
        msgBox->setText( "Не удалось обновить путь: " + query.lastError().text());
        msgBox->setIcon(QMessageBox::Critical);
        msgBox->setStyleSheet(styleSheet);
        msgBox->exec();
        return;
    }
    else{
        msgBox = new QMessageBox();
        msgBox->setWindowTitle(":)");
        msgBox->setText( "Путь успешно изменен");
        msgBox->setIcon(QMessageBox::Critical);
        msgBox->setStyleSheet(styleSheet);
        msgBox->exec();
    }
    msgBox = new QMessageBox();
    msgBox->setWindowTitle(":)");
    msgBox->setText("Изображение установлено успешно");
    msgBox->setIcon(QMessageBox::Critical);
    msgBox->setStyleSheet(styleSheet);
    msgBox->exec();
}

void settingWindow::on_pushButton_5_clicked()
{
    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();
    this->close();
}

