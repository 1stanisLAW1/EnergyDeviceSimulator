#include "telesystem.h"
#include "graphicmodul.h"
#include "qboxlayout.h"
#include "qmenu.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QGraphicsScene>
#include <QLabel>
#include <QLineEdit>

Telesystem::Telesystem(QObject *parent)
    : QObject{parent}
{
}
void Telesystem::setScene(GraphicsScene *s)
{
    if(!s){return;}
    graphicsScene = s;
    connect(graphicsScene,SIGNAL(itemSelect(QList<QGraphicsItem*>)),this,SLOT(acceptsItem(QList<QGraphicsItem*>)));
    connect(graphicsScene,SIGNAL(lkm(QList<QGraphicsItem*>)),this,SLOT(pointShow(QList<QGraphicsItem*>)));
}

bool Telesystem::appendModul(Moduls m)
{
    bool isCouplingValid = Moduls::exemCoupling(lastCoupling(), m.upCoupling());
    if (isCouplingValid) {
        Moduls* newModule = new Moduls(m);
        mModules.append(newModule);
        drawModul(newModule);
        emit updated();
    } else {
    }
    info();
    return isCouplingValid;
}

Moduls::TypeCoupling Telesystem::lastCoupling()
{
    if(mModules.isEmpty()){
        return Moduls::TypeCoupling::None;
    }
    return mModules.last()->downCoupling();
}

void Telesystem::info()
{
    int result =0;
    qDebug()<<"Telesystem Has "<<mModules.size()<<"Modules";
    for(int i = 0;i<mModules.size();i++){
        result+=mModules.at(i)->len();
    }
    qDebug()<<"Full lenght "<<result;
}

void Telesystem::graphicsInfo() const
{
    foreach (auto mod,gMod) {
        qDebug()<<mod->info();
    }
}

void Telesystem::drawModul(Moduls *mod)
{
    GraphicModul* item=new GraphicModul(mod);
    gMod.append(item);
    graphicsScene->addItem(item);
    double y = 0;
    if (mModules.size() > 1) {
        y = lenToModule(mModules.size() - 2);
    }
    item->setY(y);
    qDebug() << "drawModul" << fullLen() << lenToModule(mModules.size() - 1);
    graphicsScene->update(graphicsScene->sceneRect());
}

double Telesystem::fullLen() const
{
    double result = 0;
    for(int i =0;i <mModules.size();i++){
        result+=mModules.at(i)->len();
    }
    return result;

}

double Telesystem::lenToModule(int indx) const
{
    double result = 0;
    if(mModules.size()<indx){return 0;}
    for(int i = 0;i <=indx; i++){
        result+=mModules.at(i)->len()/10;
    }
    return result;
}

void Telesystem::acceptsItem(QList<QGraphicsItem*> graphItems)//N
{
    if (graphItems.empty() || graphItems.size() > 1) { return; }
    int i = 0;
    while(i < gMod.size()){
        if (gMod[i] == graphItems.first()) {
            qDebug()<<"List Items "<<gMod.at(i)->info();
            break;
        }
        i++;
    }
    modifiedIndex = i;
    QCursor *pos = new QCursor();
    qDebug() << "modules" << mModules.size() << i;
    QMenu *contextMenu = new QMenu();
    if (mModules[modifiedIndex]->variableLength()) {
        QAction *act = contextMenu->addAction("Изменить");
        connect(act, SIGNAL(triggered()), this, SLOT(dialog()));
    }
    else{
        emit txtEd(0, "Item can't modify its length");
    }

    QAction *act2 = contextMenu->addAction("Удалить модуль");
    QAction *act3 = contextMenu->addAction("Очистить конструктор");

    connect(act2, SIGNAL(triggered()), this, SLOT(remove()));
    connect(act3, SIGNAL(triggered()), this, SLOT(clearScene()));
    contextMenu->exec(pos->pos());
    delete(pos);
}
void Telesystem::dialog(){

    if(modifiedIndex<0){return;}
    QDialog*dialog = new QDialog();
    QString lablstr = QString("Введите длинну от %1 до %2")
                          .arg(mModules.at(modifiedIndex)->minL())
                          .arg(mModules.at(modifiedIndex)->maxL());
    QString ed = QString("%1").arg(mModules.at(modifiedIndex)->len());

    QLabel* lbl = new QLabel(lablstr,dialog);
    QLineEdit *edt = new QLineEdit(ed,dialog);
    QIntValidator *valid = new QIntValidator(mModules.at(modifiedIndex)->minL(),mModules.at(modifiedIndex)->maxL(),this);
    edt->setValidator(valid);

    QDialogButtonBox * btnBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,dialog);

    connect(btnBox,&QDialogButtonBox::accepted,dialog,&QDialog::accept);
    connect(btnBox,&QDialogButtonBox::rejected,dialog,&QDialog::reject);

    QVBoxLayout* lt = new QVBoxLayout();
    lt->addWidget(lbl);
    lt->addWidget(edt);
    lt->addWidget(btnBox);
    dialog->setLayout(lt);

    if(dialog->exec()!=QDialog::Accepted){return;}
    QString  str = edt->text();
    int lenght = str.toInt();
    if(lenght>mModules.at(modifiedIndex)->maxL()||lenght<mModules.at(modifiedIndex)->minL()){
        emit txtEd(0,"Проверьте введеную длину");
        return;
    }
    else{
        int curLen = mModules.at(modifiedIndex)->len();
        int result = curLen-lenght;
        lenght = mModules.at(modifiedIndex)->setCurrentLength(lenght);
        if(result<0)
        {
            for(int i = modifiedIndex+1;i<gMod.size();i++){
                auto pos = gMod[i]->pos();
                gMod.at(i)->setPos(0,pos.y() - result/10);
                emit updated();
            }
        }
        else if(result>0){
            for(int i = modifiedIndex+1;i<gMod.size();i++){
                auto pos = gMod[i]->pos();
                gMod.at(i)->setPos(0,pos.y() - result/10);
                emit updated();
            }
        }
        else{
            return;
        }
    }
    QList<Point> points = mModules.at(modifiedIndex)->getPoints();

    for(int i =0;i<points.size();i++){
        if(lenght>points.at(i).lent()){
            lenght = mModules.at(modifiedIndex)->setCurrentLength(lenght);
        }else{
            emit txtEd(0,"Длинна не может быть меньше позиции точки");
            break;
        }
    }
    modifiedIndex = -1;

}
void Telesystem::remove(){
    if(modifiedIndex<0||modifiedIndex>=mModules.size()){return;}
    qDebug()<<modifiedIndex;
    if(gMod.size()>=2&&modifiedIndex ==0){
        emit txtEd(0,"Удаление невозможно");
    }
    else if(modifiedIndex==mModules.size()-1){
        qDebug()<<"Delete good";
        graphicsScene->removeItem(gMod.at(modifiedIndex));
        QList<QGraphicsItem*>items =mAdditionalItems.take(gMod[modifiedIndex]);
        if(items.empty()){return;}
        for(int i = items.size()-1;i>=0;i--){
            graphicsScene->removeItem(items[i]);
            delete(items[i]);
        }
        gMod.remove(modifiedIndex);
        mModules.remove(modifiedIndex);
        qDebug()<<"delet";
        emit updated();
    }
    else{
        int firstItem = modifiedIndex - 1;
        int nextItem = modifiedIndex + 1;

        if (modifiedIndex < 0 || modifiedIndex >= mModules.size()) {
            emit txtEd(0,"Недопустимый индекс");
            return;
        }
        if (Moduls::exemCoupling(mModules.at(firstItem)->downCoupling(), mModules.at(nextItem)->upCoupling())) {
            double minus = mModules[modifiedIndex]->len();
            graphicsScene->removeItem(gMod.at(modifiedIndex));
            for (int i = modifiedIndex + 1; i < gMod.size(); i++) {
                auto pos = gMod[i]->pos();
                gMod[i]->setPos(0, pos.y() - (minus / 10));
            }

            mModules.remove(modifiedIndex);
            gMod.remove(modifiedIndex);

            QList<QGraphicsItem*> items = mAdditionalItems.take(gMod[modifiedIndex]);
            if (!items.empty()) {
                for (int i = items.size() - 1; i >= 0; i--) {
                    graphicsScene->removeItem(items[i]);
                    delete items[i];
                }
            }
            emit updated();
        } else {
            emit txtEd(0,"Удаление невозможно");
            return;
        }
        modifiedIndex = -1;
    }
}
void Telesystem::clearScene(){
    if(modifiedIndex<0){return;}
    graphicsScene->clear();
    gMod.clear();
    mModules.clear();
    emit updated();
    modifiedIndex = -1;

}
void Telesystem::pointShow(QList<QGraphicsItem*>graphicsItem){
    if(graphicsItem.empty()||graphicsItem.size()>1){return;}
    int i = 0;
    while(i<gMod.size()){
        if(gMod[i] == graphicsItem.first()){
            break;
        }
        i++;
    }
    modifiedIndex=i;
    qDebug()<<"modIndex- "<<modifiedIndex;
    if(modifiedIndex<0){return;}
    if(!mAdditionalItems.contains(gMod[modifiedIndex])){
        QList<Point>points = mModules.at(modifiedIndex)->getPoints();
        QList<QGraphicsItem*> items;
        for(int i = 0;i<points.size();i++){
            double y = 0;
            if(mModules.size()>1){
                y = lenToModule(modifiedIndex-1);
            }
            QGraphicsLineItem *line = new QGraphicsLineItem(0,y+points.at(i).lent()/10,60,y+points.at(i).lent()/10);
            mAdditionalItems[gMod[modifiedIndex]].append(line);
            items.append(line);
            QGraphicsTextItem*text= new QGraphicsTextItem(points.at(i).mname());
            mAdditionalItems[gMod[modifiedIndex]].append(text);
            items.append(text);

            text->setY(y+points.at(i).lent()/10-13);
            text->setX(60+((i*35)+20));
            graphicsScene->addItem(line);
            graphicsScene->addItem(text);
        }
        mAdditionalItems[gMod[modifiedIndex]] = items;
        qDebug()<<mAdditionalItems<<gMod[modifiedIndex];
    }else{
        QList<QGraphicsItem*>items = mAdditionalItems.take(gMod[modifiedIndex]);
        for(int a = items.size()-1;a>=0;a--){
            graphicsScene->removeItem(items[a]);
            delete(items[a]);
        }
    }
    modifiedIndex=-1;
}
