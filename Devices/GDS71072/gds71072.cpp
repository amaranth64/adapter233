#include "gds71072.h"

GDS71072::GDS71072(QString deviceName, QString port, QSerialPort::BaudRate br, QObject *parent)
    :ComPortDevice(deviceName, port, br, parent)
{
    qDebug() << "create " + getDeviceName();
}

GDS71072::~GDS71072()
{
    qDebug() << "delete "  + getDeviceName();
}

//=======================================================================

bool GDS71072::autoset()
{
    QByteArray sendString = ":AUToset\n";
    addTelemeryLine("Строка запроса: " + sendString + " автонастройка [autoset]");

    // Запись данных в порт
    return writeToPort(sendString);
}

bool GDS71072::getIDN(QString &idn)
{
    QByteArray result = "";
    QByteArray sendString = "*idn?\n";

    addTelemeryLine("Строка запроса: " + sendString + " чтение идентификационного номера [idnDevice]");

    // Запись данных в порт с ожиданием ответа.
    if (!read(sendString, result)){
        return false;
    };

    addTelemeryLine("Получен ответ: " + result.replace("\r","").replace("\n","") + " [idnDevice]");

    idn = result;
    return true;
}

bool GDS71072::getSignal(GDS71072::GDS71072Channel channel, QVector<double> &values)
{
    QByteArray sendString;
    QByteArray resp(20000, '\0');

    addTelemeryLine("Строка запроса: " + sendString + " чтение измеренного значения [getSignal]");

    switch (channel) {
    case CH1:
        sendString = ":acquire1:memory?\n";
        break;
    case CH2:
        sendString = ":acquire2:memory?\n";
        break;
    default:
        return false;
        break;
    }

    // Запись данных в порт с ожиданием ответа.
    if (!read(sendString, resp)){
        addTelemeryLine("ответ не получен [getSignal]");
        return false;
    };

    addTelemeryLine("ответ получен, идет преобразование данных [getSignal]");

    QList<QByteArray> wba = resp.split('#');
    QVector<double> result;
    QByteArray data;

    data = wba.at(0);

    // количество точек
    quint16 begin = data.indexOf("Memory Length,");
    quint16 startNum = data.indexOf(',', begin);
    quint16 stopNum = data.indexOf(';', begin);
    quint16 points = data.mid(startNum + 1, stopNum - startNum - 1).toInt();

    // вертикальная шкала
    begin = data.indexOf("Vertical Scale,");
    startNum = data.indexOf(',', begin);
    stopNum = data.indexOf(';', begin);
    double verticalScale = data.mid(startNum + 1, stopNum - startNum - 1).toFloat();

    // период sampling
    begin = data.indexOf("Sampling Period,");
    startNum = data.indexOf(',', begin);
    stopNum = data.indexOf(';', begin);
    double samplePeriod = data.mid(startNum + 1, stopNum - startNum - 1).toFloat();

    data = wba.at(1);

    quint8 p = data.at(0) - '0';
    quint16 pos = p + 1;

    // quint16 cntData = data.mid(1, p).toUInt();
    result.append(verticalScale);
    result.append(samplePeriod);

    quint8 q1, q2;
    while (pos < data.size()-2)
    {
        q1 = data.at(pos);
        q2 = data.at(pos + 1);

        int pnt = q1 * 256 + q2;
        if (pnt >= 32768) pnt = pnt - 65535;
        result.append( pnt * verticalScale / 25);
        pos= pos + 2;
    }

    values.clear();
    values.append(result);

    addTelemeryLine("преобразование данных завершено! [getSignal]");
    return true;



}

bool GDS71072::getFrequency(GDS71072::GDS71072Channel channel, double &freq)
{
    double result = 0.0;

    if (!setSourceChannel(channel)){
        return false;
    };

    QByteArray sendString = ":MEAS:FREQ?\n";
    QByteArray data;

    // Запись данных в порт с ожиданием ответа. Ответ пишется в data
    if (!read(sendString, data)){
        return false;
    };

    bool bOk;
    result = data.toDouble(&bOk); // ok == true если все нормально

    if (!bOk){
         addTelemeryLine("Ошибка преобразования данных из строки в число!");
         return false;
     }

    addTelemeryLine("Получен ответ: " + data.replace("\r","").replace("\n","") + " [readVoltage]");

    freq = result;
    return true;
}

bool GDS71072::getAmplitude(GDS71072::GDS71072Channel channel, double &ampl)
{
    double result = 0.0;

    if (!setSourceChannel(channel)){
        return false;
    };

    QByteArray sendString = ":MEAS:AMP?\n";
    QByteArray data;

    // Запись данных в порт с ожиданием ответа. Ответ пишется в data
    if (!read(sendString, data)){
        return false;
    };

    bool bOk;
    result = data.toDouble(&bOk); // ok == true если все нормально

    if (!bOk){
         addTelemeryLine("Ошибка преобразования данных из строки в число!");
         return false;
     }

    addTelemeryLine("Получен ответ: " + data.replace("\r","").replace("\n","") + " [readVoltage]");

    ampl = result;
    return true;
}

bool GDS71072::getPeriod(GDS71072::GDS71072Channel channel, double &per)
{
    double result = 0.0;

    if (!setSourceChannel(channel)){
        return false;
    };

    QByteArray sendString = ":measure:period?\n";
    QByteArray data;

    // Запись данных в порт с ожиданием ответа. Ответ пишется в data
    if (!read(sendString, data)){
        return false;
    };

    bool bOk;
    result = data.toDouble(&bOk); // ok == true если все нормально

    if (!bOk){
         addTelemeryLine("Ошибка преобразования данных из строки в число!");
         return false;
     }

    addTelemeryLine("Получен ответ: " + data.replace("\r","").replace("\n","") + " [readVoltage]");

    per = result;
    return result;
}

//=======================================================================

bool GDS71072::setSourceChannel(GDS71072::GDS71072Channel channel)
{
    QByteArray sendString;
    switch (channel) {
        case CH1:
            sendString = ":MEAS:SOUR1 CH1\n";
            break;
        case CH2:
            sendString = ":MEAS:SOUR1 CH2\n";
            break;
        default:
            return false;
        }
    writeToPort(sendString);

    Sleep(100);
    return true;
}
