#include "historykonstruct.h"
#include "mainwindow.h"
#include "qapplication.h"
#include "qboxlayout.h"
#include "qdebug.h"
#include "qlineedit.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "qregularexpression.h"
#include "qscrollarea.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QDir>
#include <QFile>
#include <QRegularExpression>



HistoryKonstruct::HistoryKonstruct():tm(new Telesystem){
    qDebug() << "_________________________________Объект HistoryKonstruct создан_______________________________________________";
    dirPath = "C:/Users/Neofit/Konstructor/DB";
    bool isConnected = connect(tm, &Telesystem::modSignal, this, &HistoryKonstruct::mod);
    qDebug() << "Подключение сигнала к слоту:" << (isConnected ? "успешно" : "не удалось");

    qDebug() << "Поток Telesystem:" << tm->thread();
    qDebug() << "Поток HistoryKonstruct:" << this->thread();

    qDebug() << "Объект Telesystem существует:" << (tm != nullptr);
}

void HistoryKonstruct::onBtnClickedDialog()
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

void HistoryKonstruct::onBtnClickedSave()
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
            // if (streamText(LD->text())) {
            //     dialog->accept();
            // }
            dialog->accept();
        }
    });
    dialog->exec();
}

void HistoryKonstruct::mod(QString str)
{
    qDebug()<<"SOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOS";
    qDebug() << "Получено сообщение:" << str;
}

// bool HistoryKonstruct::streamText(QString path)
// {
//     qDebug() << "Before append - SIZE:" << tm->backMod().size();
//     QList<Moduls*>& mModules = tm->backMod(); // Используем существующий объект tm
//     mModules.append(new Moduls);

//     qDebug() << "After append - SIZE:" << mModules.size();
//     for (int i = 0; i < mModules.size(); i++) {
//         qDebug() << mModules.at(i)->name();
//     }
//     return true;
// }

