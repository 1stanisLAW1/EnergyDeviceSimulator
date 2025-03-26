#include "delet.h"
#include "ui_delet.h"
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
