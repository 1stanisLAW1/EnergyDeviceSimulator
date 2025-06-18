#include "mainfunctions.h"
#include "qdebug.h"
#include "QDate"
#include "qlineedit.h"
#include "qmessagebox.h"
#include "qregularexpression.h"

#include <QString>

Mainfunctions::Mainfunctions(QObject *parent)
    : QObject{parent}
{
    lw = new LogiWrite();
}

Mainfunctions::~Mainfunctions()
{
    delete lw;
}

QString Mainfunctions::command_0x19(QByteArray requestData)
{
    if (requestData.size() != 15 || static_cast<quint8>(requestData.at(0)) != 137 || static_cast<quint8>(requestData.at(2)) != 25) {
        qDebug() << "Неверный формат команды!";
        return QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss");
    }

    quint8 sec = requestData.at(5);
    quint8 min = requestData.at(6);
    quint8 hour = requestData.at(7);
    quint8 day = requestData.at(9);
    quint8 month = requestData.at(10);
    quint16 year = (static_cast<quint8>(requestData.at(12)) << 8) | static_cast<quint8>(requestData.at(11));

    return QString("%1.%2.%3 %4:%5:%6")
        .arg(day, 2, 10, QLatin1Char('0'))   // день с ведущим нулём
        .arg(month, 2, 10, QLatin1Char('0'))  // месяц с ведущим нулём
        .arg(year)
        .arg(hour, 2, 10, QLatin1Char('0'))   // часы с ведущим нулём
        .arg(min, 2, 10, QLatin1Char('0'))    // минуты с ведущим нулём
        .arg(sec, 2, 10, QLatin1Char('0'));   // секунды с ведущим нулём
}

QByteArray Mainfunctions::answer_command_v2(QByteArray request)
{
    QByteArray response;
    QByteArray array;

    response.append(1, static_cast<char>(adress));
    response.append(request.at(0));
    response.append(static_cast<char>(0x00));  // Флаги

    quint8 commandNum = static_cast<quint8>(request[2]);
    response.append(static_cast<char>(0x0B));
    response.append(static_cast<char>(0x00));

    switch (commandNum) {

    case 0x01:
    {
        for (int i = 0; i < 16; i++) {
            float density = values.at(i).toFloat();
            char* densityBytes = reinterpret_cast<char*>(&density);
            array.append(densityBytes, 4);
        }

        for (int i = 16; i < 32; i++) {
            float ncValue = values.at(i).toFloat()*20;
            char* ncBytes = reinterpret_cast<char*>(&ncValue);
            array.append(ncBytes, 4);
        }
    }
    break;
    case 0x02:
    {
        float dta = values.at(32).toFloat();
        float dtb = values.at(33).toFloat();
        float dtc = values.at(34).toFloat();
        float dtd = values.at(35).toFloat();
        float dte = values.at(36).toFloat();
        float dtf = values.at(37).toFloat();
        float dtg = values.at(38).toFloat();
        float dth = values.at(39).toFloat();
        float w = values.at(46).toFloat();

        appendFloat(array, dta);
        appendFloat(array, dtb);
        appendFloat(array, dtc);
        appendFloat(array, dtd);
        appendFloat(array, dte);
        appendFloat(array, dtf);
        appendFloat(array, dtg);
        appendFloat(array, dth);
        appendFloat(array, w);
    }
        break;
    case 0x03:
    {
        float dtup = values.at(40).toFloat();
        float dtwr = values.at(41).toFloat();
        float dtdn = values.at(42).toFloat();
        float dtlf = values.at(43).toFloat();
        float w = values.at(46).toFloat();

        char* cdtup = reinterpret_cast<char*>(&dtup);
        char* cdtwr = reinterpret_cast<char*>(&dtwr);
        char* cdtdn = reinterpret_cast<char*>(&dtdn);
        char* cdtlf = reinterpret_cast<char*>(&dtlf);
        char* cw = reinterpret_cast<char*>(&w);

        array.append(cdtup, 4);
        array.append(cdtwr, 4);
        array.append(cdtdn, 4);
        array.append(cdtlf, 4);
        array.append(cw, 4);
    }
        break;
    case 0x04:{
        float top = values.at(44).toFloat();
        float bot = values.at(45).toFloat();
        float w = values.at(46).toFloat();

        appendFloat(array, top);
        appendFloat(array, bot);
        appendFloat(array, w);
    }
    break;
    case 0x05:{
        float den = values.at(47).toFloat();  //4
        char n = values.at(48).toFloat();    //1
        float w = values.at(46).toFloat();    // 4

        char* cDEN = reinterpret_cast<char*>(&den);
        char* cW = reinterpret_cast<char*>(&w);

        array.append(cDEN, 4);
        array.append(n);
        array.append(cW, 4);
    }break;
    case 0x06:{
        signed char DRSR = '\0';
        signed char RDSR  = '\0';

        for(int i = 49;i<65;i++){
            signed char DR  = values.at(i).toInt()*100;
            DRSR+=DR;
            array.append(DR);
        }
        for(int i = 65;i<81;i++){
            signed char RD  = values.at(i).toInt();
            RDSR+=RD;
            array.append(RD);
        }

        array.append(DRSR/16);
        array.append(RDSR/16);
    }
        break;
    case 0x07:
    {

        uint16_t znt = values.at(81).toInt()* 10;
        array.append(static_cast<char>(znt & 0xFF));
        array.append(static_cast<char>((znt >> 8) & 0xFF));

        int16_t crv = values.at(82).toInt()*10;
        array.append(static_cast<char>(crv & 0xFF));
        array.append(static_cast<char>((crv >> 8) & 0xFF));

        // RSV  4 байта
        array.append(static_cast<char>(0x00));
        array.append(static_cast<char>(0x00));
        array.append(static_cast<char>(0x00));
        array.append(static_cast<char>(0x00));
    }
    break;
    case 0x08:
    {
        float IGK_1 = values.at(83).toFloat();
        float IGK_2 = values.at(84).toFloat();
        float IGK_3 = values.at(85).toFloat();

        appendFloat(array,IGK_1);
        appendFloat(array,IGK_2);
        appendFloat(array,IGK_3);
    }
        break;
    case 0x0A:
    {
        uint16_t BA = values.at(86).toInt()*10;
        uint16_t CS = values.at(87).toInt()*10;
        uint16_t PE = values.at(88).toInt()*100;
        uint16_t DAC  = values.at(89).toInt();
        uint16_t CA  = values.at(90).toInt();

        array.append(static_cast<char>(BA >> 8));
        array.append(static_cast<char>(BA & 0xFF));
        array.append(static_cast<char>(CS >> 8));
        array.append(static_cast<char>(CS & 0xFF));
        array.append(static_cast<char>(PE >> 8));
        array.append(static_cast<char>(PE & 0xFF));
        array.append(static_cast<char>(DAC >> 8));
        array.append(static_cast<char>(DAC & 0xFF));
        array.append(static_cast<char>(CA));
    }
        break;
    case 0x0B:
    {
        for (int i = 91; i < 107; i++) {
            array.append(static_cast<char>(values.at(i).toInt()));
        }
        quint16 dmin = values.at(107).toInt();;
        quint16 dmax = values.at(108).toInt();;
        quint16 davg = (dmin+dmax)/2;

        array.append(static_cast<char>(dmin >> 8));
        array.append(static_cast<char>(dmin & 0xFF));
        array.append(static_cast<char>(dmax >> 8));
        array.append(static_cast<char>(dmax & 0xFF));
        array.append(static_cast<char>(davg >> 8));
        array.append(static_cast<char>(davg & 0xFF));
    }
        break;
    case 0x0C:
    {
        float prs_1 = values.at(109).toFloat();
        float prs_2 = values.at(110).toFloat();
        uint16_t tmp_1 = values.at(111).toInt();
        uint16_t tmp_2 = values.at(112).toInt();

        appendFloat(array, prs_1);
        appendFloat(array, prs_2);
        array.append(static_cast<char>(tmp_1 >> 8));
        array.append(static_cast<char>(tmp_1 & 0xFF));
        array.append(static_cast<char>(tmp_2 >> 8));
        array.append(static_cast<char>(tmp_2 & 0xFF));
    }
        break;
    case 0x0D:
    {
        for (int i = 91; i < 107; i++) {
            array.append(static_cast<char>(values.at(i).toInt()));
        }
        quint16 dmin = values.at(107).toInt();;
        quint16 dmax = values.at(108).toInt();;
        quint16 davg = (dmin+dmax)/2;

        array.append(static_cast<char>(dmin >> 8));
        array.append(static_cast<char>(dmin & 0xFF));
        array.append(static_cast<char>(dmax >> 8));
        array.append(static_cast<char>(dmax & 0xFF));
        array.append(static_cast<char>(davg >> 8));
        array.append(static_cast<char>(davg & 0xFF));
    }
        break;
    case 0x0E:
    {
        float near = values.at(113).toFloat();
        float nfar = values.at(114).toFloat();

        appendFloat(array, near);
        appendFloat(array, nfar);
    }
        break;
    case 0x18:{
        QDateTime current = QDateTime::currentDateTime();

        array.append(static_cast<char>(current.time().second()));
        array.append(static_cast<char>(current.time().minute()));
        array.append(static_cast<char>(current.time().hour()));

        int dayOfWeek = current.date().dayOfWeek();
        dayOfWeek = (dayOfWeek == 7) ? 1 : dayOfWeek + 1;
        array.append(static_cast<char>(dayOfWeek));

        array.append(static_cast<char>(current.date().day()));
        array.append(static_cast<char>(current.date().month()));

        int year = current.date().year();

        array.append(static_cast<char>(year & 0xFF));
        array.append(static_cast<char>((year >> 8) & 0xFF));
    }break;
    case 0xFC:{
        QString str = values.at(117);
        if(str.size()<4){
            for(int i = str.size();i<4;i++){
                str+="0";
            }
        }
        QByteArray latin1 = str.left(4).toLatin1();  // Берём первые 4 символа и конвертируем
        array.append(latin1.constData(), latin1.size());

        int16_t num = values.at(115).toInt();
        float rsrv = values.at(116).toFloat();
        char* rsrvBytes = reinterpret_cast<char*>(&rsrv);

        array.append(static_cast<char>(num & 0xFF));
        array.append(static_cast<char>(num >> 8));
        array.append(rsrvBytes, 4);
    }break;
    default: break;
    }

    array.append(static_cast<char>(0xFF));//status 1
    array.append(static_cast<char>(0xFF));
    array.append(static_cast<char>(0x00));//status2

    int16_t N_data = array.size();
    response[3] = static_cast<char>(N_data & 0xFF);       // Младший байт
    response[2] = static_cast<char>((N_data >> 8) & 0xFF); // Старший байт

    response.append(array);

    quint8 checksum = 0;

    for(int i = 0; i < response.size(); i++) {
        quint8 byte = static_cast<quint8>(response.at(i));
        checksum ^= byte;
        bool lsb = checksum & 0x01;
        checksum >>= 1;
        if(lsb) checksum |= 0x80;
        if(checksum > 127) checksum ^= 0x3C;
    }
    response.append(checksum);  // Контрольная сумма
    response.append(static_cast<char>(0xFF));

    return response;
}

void Mainfunctions::appendFloat(QByteArray &array, float value)
{
    const char* bytes = reinterpret_cast<const char*>(&value);
    for(size_t i = 0; i < sizeof(float); ++i) {
        array.append(bytes[i]);
    }
}

void Mainfunctions::appendList(QList<QWidget *> list)
{
    QList<QLineEdit*>list_line;
    for (QWidget *widget : list) {
        list_line += widget->findChildren<QLineEdit*>();
    }
    int indx = -1;
    bool ok;
    for(int i = 0; i < list_line.size()-1; i++) {
        QString text = list_line.at(i)->text();
        text.toInt(&ok);

        if(!ok) {
            QMessageBox msgBox;
            msgBox.setText("Ошибка преобразования в число:" + text);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setWindowTitle("Error!");
            msgBox.exec();

            emit valid();
            values.clear();
            return;
        }

        values.append(text);
        indx++;
    }
    if(isLatinAndDigits(list_line.at(117)->text())==false){
        QMessageBox msgBox;
        msgBox.setText("Ошибка некорректные данные:" + list_line.at(117)->text());
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Error!");
        msgBox.exec();

        emit valid();
        values.clear();
        return;
    }
    values.append(list_line.at(list_line.size()-1)->text());
}

void Mainfunctions::clearVec()
{
    values.clear();
}

void Mainfunctions::setAdres(QString a)
{

    bool ok;
    int value = a.toInt(&ok, 16);
    if (ok) {
        adress = value;
    } else {
        qDebug() << "Не удалось преобразовать строку в число:" << a;
    }
}

bool Mainfunctions::isLatinAndDigits(const QString &str)
{
    QRegularExpression regex("^[A-Za-z0-9]+$"); // только латиница и цифры
    if(str.size()!=4){return false;}
    return regex.match(str).hasMatch();
}





