#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qboxlayout.h"
#include "qlineedit.h"
#include "sendingcommands.h"
#include "upwidg.h"
#include <QLabel>
#include <QMainWindow>

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

private slots:
    void appendList();

private:
    Ui::MainWindow *ui;
    UpWidg*upW;

    QVBoxLayout* vLayout;
    QLineEdit*lineEdit;
    QLabel* lblLeftPanel_2;
    SendingCommands*sComm;
    QTabWidget* tabWidget;

};
#endif // MAINWINDOW_H
