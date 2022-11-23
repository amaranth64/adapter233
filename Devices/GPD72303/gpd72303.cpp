#include "gpd72303.h"

GPD72303::GPD72303(QString deviceName, QString port, QSerialPort::BaudRate br, QObject *parent)
    :ComPortDevice(deviceName, port, br, parent)
{
    qDebug() << "create " + getDeviceName();
}

GPD72303::~GPD72303()
{
    qDebug() << "delete "  + getDeviceName();
}

bool GPD72303::setVoltage(double V)
{
    QByteArray sendString = "VSET1:" + sendString.number(V, 'g', 2) + "\r";
    addTelemeryLine("Строка запроса: " + sendString + " установка напряжения " + QString::number(V) + "V [setVoltage]");

    // Запись данных в порт
    return writeToPort(sendString);
}

bool GPD72303::setVoltageLimit(double Ulim)
{
    bool result;
    QByteArray sendString = "VSET1:" + sendString.number(Ulim, 'g', 2) + "\r";
    addTelemeryLine("Строка запроса: " + sendString +
                    " установка ограничения напряжения канал 1 " + QString::number(Ulim) + "V [setVoltage]");

    // Запись данных в порт
    result = writeToPort(sendString);

    sendString = "VSET2:" + sendString.number(Ulim, 'g', 2) + "\r";
    addTelemeryLine("Строка запроса: " + sendString +
                    " установка ограничения напряжения канал 2 -" + QString::number(Ulim) + "V [setVoltage]");

    // Запись данных в порт
    result = result & writeToPort(sendString);

    return result;
}

bool GPD72303::setCurrentLimit(double Ilim)
{
    bool result;
    QByteArray sendString = "ISET1:" + sendString.number(Ilim, 'g', 2) + "\r";
    addTelemeryLine("Строка запроса: " + sendString +
                    " установка ограничения напряжения канал 1 " + QString::number(Ilim) + "V [setVoltage]");

    // Запись данных в порт
    result = writeToPort(sendString);

    sendString = "ISET2:" + sendString.number(Ilim, 'g', 2) + "\r";
    addTelemeryLine("Строка запроса: " + sendString +
                    " установка ограничения напряжения канал 2 -" + QString::number(Ilim) + "V [setVoltage]");

    // Запись данных в порт
    result = result & writeToPort(sendString);

    return result;
}

bool GPD72303::setOutput(GPD72303::GPD72303State state)
{
    QByteArray sendString;

    // Включение-выключение выхода ИП
    if (state == ON){
        sendString = "OUT1\r";
        addTelemeryLine("Включаем источник питания - ON");
    } else {
        sendString = "OUT0\r";
        addTelemeryLine("Выключаем источник питания - OFF");
    }

    return writeToPort(sendString);
}

bool GPD72303::readVoltage(double &voltage)
{
    double result = 0.0;
    QByteArray data;
    QByteArray sendString = "VOUT1?\r";

    addTelemeryLine("Чтение установленного напряжения [readVoltage]");

    // Запись данных в порт с ожиданием ответа. Ответ пишется в data
    if (!read(sendString, data)){
        return false;
    };

    // Преобразуем строку с данными в число d и заносим в поле dataValue
    data.remove(0,1);

    bool bOk;
    result = data.toDouble(&bOk); // ok == true если все нормально

    if (!bOk){
         addTelemeryLine("Ошибка преобразования данных из строки в число!");
         return false;
     }

     addTelemeryLine("Получен ответ: " + data.replace("\r","").replace("\n","") + " [readVoltage]");

     voltage = result;
      return true;
}

bool GPD72303::readCurrent(double &current)
{
    double result = 0.0;
    QByteArray data;
    QByteArray sendString = "IOUT1?\r";

    addTelemeryLine("Чтение установленного напряжения [readCurrent]");

    // Запись данных в порт с ожиданием ответа. Ответ пишется в data
    if (!read(sendString, data)){

        return false;
    };

    // Преобразуем строку с данными в число d и заносим в поле dataValue
    data.remove(0,1);

    bool bOk;
    result = data.toDouble(&bOk); // ok == true если все нормально

    if (!bOk){
        addTelemeryLine("Ошибка преобразования данных из строки в число!");
        return false;
    }


    addTelemeryLine("Получен ответ: " + data.replace("\r","").replace("\n","") + " [readCurrent]");
    current = result;
    return true;
}
