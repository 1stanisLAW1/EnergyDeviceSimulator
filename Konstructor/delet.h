#ifndef DELET_H
#define DELET_H

#include <QDialog>

namespace Ui {
class delet;
}

class delet : public QDialog
{
    Q_OBJECT

public:
    explicit delet(QWidget *parent = nullptr);
    ~delet();
private slots:
    void on_pushButton_clicked();

private:
    Ui::delet *ui;
};

#endif // DELET_H
