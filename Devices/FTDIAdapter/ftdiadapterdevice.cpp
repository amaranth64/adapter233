#include "ftdiadapterdevice.h"

FTDIAdapterDevice::FTDIAdapterDevice(QString deviceName, QString port, QSerialPort::BaudRate br, QObject *parent)
    :ComPortDevice(deviceName, port, br, parent)
{
    qDebug() << "create " + getDeviceName();
}

FTDIAdapterDevice::~FTDIAdapterDevice()
{
    qDebug() << "delete "  + getDeviceName();
}

//================================================================================

bool FTDIAdapterDevice::checkConnection()
{
    // сделать запрос на соединение с адаптером
    QByteArray data;
    QByteArray sendString = "getPortA;";

    addTelemeryLine("Проверка связи с адаптером [checkConnection]");

    // Запись данных в порт с ожиданием ответа. Ответ пишется в data
    if (!read(sendString, data)){
        addErrorLine("Ошибка связи с адаптером! [checkConnection]");
        return false;
    };


    if (!data.contains("A")) {
        addTelemeryLine("Получен ответ: " + data.replace("\r","") + " [checkConnection]");
        addErrorLine("Ошибка связи с адаптером! [checkConnection]");
        return false;
    }

    addTelemeryLine("Связь с адаптером установлена!");

    return true;
}

bool FTDIAdapterDevice::setPort(const Port &port)
{
    QByteArray sendString;
    QByteArray result;

    sendString = "set" + port.toString().toLocal8Bit();

    addTelemeryLine("Установка данных в порт " + sendString + " [setPort]");


    // Запись данных в порт с ожиданием ответа. Ответ пишется в data
    if (!read(sendString, result)){
        return false;
    };

    QString a = sendString.mid(7,4);
    qDebug() << a;
    if (!result.contains(a.toLocal8Bit())) {
        addTelemeryLine("Ошибка! Ответ на запрос " + sendString + "=" + result);
        return false;
    }

    addTelemeryLine("Получен ответ: " + result.replace("\r","").replace("\n","") + " [setPort]");
    return true;
}

bool FTDIAdapterDevice::getPort(FTDIAdapterDevice::Port &port)
{
    QByteArray sendString;
    QByteArray result;

    //QBitArray p(8, false);

    // Запись данных в порт с ожиданием ответа. Ответ пишется в data
    if (!read(sendString, result)){
        return false;
    };

    // проверка корректности ответа
    if ((result.isEmpty()) | (result.count()<6)) {
        addTelemeryLine("Ошибка! Ответ на запрос " + sendString + "=" + result);
        return false;
    }

    QString a = result.mid(2,3); // вырезаем данные

    // преобразуем в число
    bool bOk;
    quint8 data = a.toInt(&bOk);

    if (!bOk) {
        addTelemeryLine("Ошибка преобразования " + sendString + " в число");
        return false;
    }

    addTelemeryLine("Получен ответ: " + result.replace("\r","").replace("\n","") + " [getPort]");

    // преобразуем к массиву бит
    port.set(data);
    return true;
}

//================================================================================

FTDIAdapterDevice::Port::Port(QString name):portName(name)
{
    baPort.resize(8);
    baPort.fill(false);
}

FTDIAdapterDevice::Port::~Port()
{
    baPort.clear();
}

QString FTDIAdapterDevice::Port::getName() const
{
    return portName;
}

void FTDIAdapterDevice::Port::set(quint8 bit, bool state)
{
    baPort[bit] = state;
}

void FTDIAdapterDevice::Port::set(quint8 value)
{
    byteToQBitArray(value,baPort);
}

bool FTDIAdapterDevice::Port::get(quint8 bit)
{
    return baPort[bit];
}

void FTDIAdapterDevice::Port::clear()
{
    baPort.fill(false);
}

QString FTDIAdapterDevice::Port::toString() const
{
    QString result;
    quint8 state = 0;
    state = bitsArrayToByte(baPort);

    result = getName() + QByteArray::number(state).rightJustified(3, '0') + ";";

    return result;
}

quint8 FTDIAdapterDevice::Port::bitsArrayToByte(QBitArray bA)
{
    quint8 byte = 0;

    for(quint8 i = 0; i < bA.count(); ++i)
        byte = (byte | ((bA[i] ? 1 : 0) << (i % 8)));

    return byte;
}

void FTDIAdapterDevice::Port::byteToQBitArray(quint8 num, QBitArray &data)
{
    for (quint8 i = 0; i < 8; ++i) data.setBit(i, num & (1 << i));
}
