#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Telesystem.h"
#include "iniloader.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QTableWidget>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getModules();
    void showModules();
    QList <Moduls> mModules;
    QList <Moduls> ModulDowload;
public slots:
    void showMessage(int e,QString str);
private slots:
    void upDateTable();
    void onBtnClicked();

    void onBtnClickedDialog();
    void onBtnClickedSave();
    bool streamText(QString);
    void loadFromTxtFile(QString);
    void load();
private:
    Ui::MainWindow *ui;
    GraphicsScene *scene;
    QWidget *modulList;
    QTableWidget *table;
    Iniloader* mIniLoader;
    Telesystem mTelSystem;
    QTextEdit * txtEdit;
    void clearTable();
    QString dirPath;
};
#endif // MAINWINDOW_H
