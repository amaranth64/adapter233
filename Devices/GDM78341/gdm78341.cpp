#include "gdm78341.h"

GDM78341::GDM78341(QString deviceName, QString port, QSerialPort::BaudRate br, QObject *parent)
    :ComPortDevice(deviceName, port, br, parent)
{
    qDebug() << "create " + getDeviceName();
}

GDM78341::~GDM78341()
{
    qDebug() << "delete "  + getDeviceName();
}

//====================================================================

bool GDM78341::clearDevice()
{
    QByteArray sendString = "*CLS\n";
    addTelemeryLine("Строка запроса: " + sendString + " очистка устройства [clearDevice]");

    // Запись данных в порт
    return writeToPort(sendString);
}

bool GDM78341::idnDevice(QString &idn)
{
    QByteArray result = "";
    QByteArray sendString = "*IDN?\n";

    addTelemeryLine("Строка запроса: " + sendString + " чтение идентификационного номера [idnDevice]");

    // Запись данных в порт с ожиданием ответа. Ответ пишется в data
    if (!read(sendString, result)){
        return false;
    };

    addTelemeryLine("Получен ответ: " + result.replace("\r","").replace("\n","") + " [idnDevice]");

    idn = result;
    return true;
}

bool GDM78341::setRegime(GDM78341::GDM78341Regime regime, QByteArray dia)
{
    QString data;
    if (!dia.isEmpty()) dia = " " + dia; // если задали диапазон нужно добавить пробел

    QByteArray sendString;

    switch (regime){
    case DC:
        sendString = "CONF:VOLT:DC" + dia + "\n";
        break;
    case AC:
        sendString = "CONF:VOLT:AC" + dia + "\n";
        break;
    case FREQ:
        sendString = "CONF:FREQ\n";
        break;
    case PER:
        sendString = "CONF:PER\n";
        break;
    case CONT:
        sendString = "CONF:CONT\n";
        break;
    default:
        return false;
    }

    addTelemeryLine("Строка запроса: " + sendString + " установка режима мультиметра [setRegime]");


    // Запись данных в порт
    if (!writeToPort(sendString)){
        return false;
    }

    currentRegime = regime;
    return true;
}

GDM78341::GDM78341Regime GDM78341::getRegime()
{
    return currentRegime;
}

bool GDM78341::readValue(double &value)
{
    double result = 0.0;
    QByteArray data;
    QByteArray sendString = ":VAL?\n";

    addTelemeryLine("Строка запроса: " + sendString + " чтение измеренного значения [readValue]");

    // Запись данных в порт с ожиданием ответа. Ответ пишется в data
    if (!read(sendString, data)){
        return false;
    };

    // Преобразуем строку с данными в число
    bool bOk;
    result = data.toDouble(&bOk); // ok == true если все нормально

    if (!bOk){
         addTelemeryLine("Ошибка преобразования данных из строки в число! [readValue]");
         addErrorLine("Ошибка преобразования данных из строки " + data + " в число!");
         return false;
     }

     addTelemeryLine("Получен ответ: " + data.replace("\r","").replace("\n","") + " [readValue]");

     value = result;
     return true;
}
