#include "psp405.h"

PSP405::PSP405(QString deviceName, QString port, QSerialPort::BaudRate br, QObject *parent)
    :ComPortDevice(deviceName, port, br, parent)
{
    qDebug() << "create " + getDeviceName();
}

PSP405::~PSP405()
{
    qDebug() << "delete "  + getDeviceName();
}

bool PSP405::setVoltage(double V)
{
    QByteArray sendString = "SV " + sendString.number(V, 'g', 3).rightJustified(5, '0') + "\r";
    addTelemeryLine("Строка запроса: " + sendString + " установка напряжения " + QString::number(V) + "V [setVoltage]");

    // Запись данных в порт
    return writeToPort(sendString);
}

bool PSP405::setVoltageLimit(double Ulim)
{
    QByteArray sendString = "SU " + sendString.number(Ulim, 'g', 3).rightJustified(5, '0') + "\r";
    addTelemeryLine("Строка запроса: " + sendString +
                    " установка ограничения напряжения" + QString::number(Ulim) + "V [setVoltage]");

    // Запись данных в порт
    return writeToPort(sendString);
}

bool PSP405::setCurrentLimit(double Ilim)
{
    QByteArray sendString = "SI " + sendString.number(Ilim, 'g', 2).rightJustified(4, '0') + "\r";
    addTelemeryLine("Строка запроса: " + sendString +
                    " установка ограничения тока" + QString::number(Ilim) + "V [setVoltage]");

    // Запись данных в порт
    return writeToPort(sendString);
}

bool PSP405::setOutput(PSP405::PSP405State state)
{
    QByteArray sendString;

    // Включение-выключение выхода ИП
    if (state == ON){
        sendString = "KOE\r";
        addTelemeryLine("Включаем источник питания - ON");
    } else {
        sendString = "KOD\r";
        addTelemeryLine("Выключаем источник питания - OFF");
    }

    return writeToPort(sendString);
}

bool PSP405::readVoltage(double &voltage)
{
    double result = 0.0;
    QByteArray data;
    QByteArray sendString = "V\r";

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

bool PSP405::readCurrent(double &current)
{
    double result = 0.0;
    QByteArray data;
    QByteArray sendString = "A\r";

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
