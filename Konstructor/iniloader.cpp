#include "iniloader.h"

Iniloader::Iniloader(QObject *parent)
    : QObject{parent}
{}

Moduls Iniloader::createModel(QString filepath)
{
    if(!QFileInfo::exists(filepath)){
        emit sendMessage(0,"not exists "+ filepath);
        return Moduls();
    }

    QSettings setting(filepath,QSettings::IniFormat);
    QStringList groupList = setting.childGroups();
    if(!groupList.contains("modul")){
        emit sendMessage(0,"empty groups " + filepath);

        return Moduls();
    }
    bool ok = true;
    QString name = setting.value("modul/name").toString();
    QString upCoupling = setting.value("modul/upCoupling").toString();
    QString downCoupling = setting.value("modul/downCoupling").toString();
    QString len = setting.value("modul/length").toString();
    QString max = setting.value("modul/maxL").toString();
    QString min = setting.value("modul/minL").toString();

    int minL= min.toInt(&ok);
    int maxL = max.toInt(&ok);
    int lenght = len.toInt(&ok);
    Moduls m(name,lenght,maxL,minL);
    if(m.maxL()>=m.len()&&m.minL()<=m.len()){
        m.setUpDown(convertStr2Type(upCoupling),convertStr2Type(downCoupling));
    }else{
    emit sendMessage(0,"Invalid length data entered:"+filepath);
    }
    for (int i = 0;i<groupList.size();i++){
        if(!groupList[i].startsWith("point")){continue;}
        QString groupName  = groupList[i];
        int lent  = setting.value(groupName+"/len").toInt();
        QString nameM = setting.value(groupName + "/mpName").toString();
        QString name = setting.value(groupName + "/mName").toString();
        Point po = Point(name,nameM,lent);
        if(po.lent()>=0&&po.lent()<=m.maxL()){
            m.addPoint(po);
        }else{
            emit sendMessage(0,"Invalid point length data entered: "+filepath);
        }
    }
    return m;
}

QList<Moduls> Iniloader::createModuls(QString dirpath)
{
    QList<Moduls> moduls;
    QFileInfo dirInfo(dirpath);
    qDebug() << dirInfo.absoluteFilePath();
    if(!dirInfo.exists()||!dirInfo.isDir()){return moduls;}
    QDir dir(dirpath);
    QStringList filter;
    filter << QString("*.ini");
    QStringList files = dir.entryList(filter,QDir::Files);

    for(int i=0;i<files.size();i++)
    {
        Moduls m = createModel(dir.absoluteFilePath(files[i]));
        if(m.isValid()){
            moduls.append(m);
        }
    }

    foreach(auto m, moduls) {
        qDebug() << m.name() << type2str(m.upCoupling()) << type2str( m.downCoupling());
    }

    qDebug() << "iniloaderCompleate";
    return moduls;
}

Moduls::TypeCoupling Iniloader::convertStr2Type(const QString &str)
{
    qDebug()<<"convertStr2Type";
    Moduls::TypeCoupling ret;
    if (str == "TOP") {
        ret = Moduls::TypeCoupling::TOP;
    }
    else if (str =="KORVET_MRKp"){
        ret = Moduls::TypeCoupling::KORVET_MRKp;
    }
    else if (str =="KORVET_MRKm"){
        ret = Moduls::TypeCoupling::KORVET_MRKm;
    }
    else if(str=="KORVET_M"){
        ret = Moduls::TypeCoupling::KORVET_M;
    }
    else if (str=="KORVET_P"){
        ret = Moduls::TypeCoupling::KORVET_P;
    }
    else if (str=="BOTTOM"){
        ret = Moduls::TypeCoupling::BOTTOM;
    }
    else {

        ret = Moduls::TypeCoupling::None;
    }
    return ret;
}
