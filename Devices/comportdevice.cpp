#include "comportdevice.h"

ComPortDevice::ComPortDevice(QString deviceName, QObject *parent):Device(deviceName, parent)
{
    comPort = new QSerialPort(this);
    qDebug() << "create ComPortDevice of " + getDeviceName();
}

ComPortDevice::ComPortDevice(QString deviceName, QString port, QObject *parent): ComPortDevice(deviceName, parent)
{
    this->comPortName = port;
}

ComPortDevice::ComPortDevice(QString deviceName, QString port, QSerialPort::BaudRate br, QObject *parent): ComPortDevice(deviceName, port, parent)
{
    this->baudRate = br;
}

ComPortDevice::~ComPortDevice()
{
    if(comPort !=nullptr){
        if (comPort->isOpen()) {
            comPort->close();
        }
    }
    delete comPort;
    qDebug() << "delete ComPortDevice of "  + getDeviceName();;
}

//====================================================================

bool ComPortDevice::openDevice()
{
    if (comPort == nullptr){
        addErrorLine("Ошибка создания экземпляра устройства!");
        addTelemeryLine("Ошибка создания экземпляра устройства [openDevice]");
        return false;
    }

    if ((baudRate == QSerialPort::UnknownBaud)|(comPortName.isEmpty())){
        addErrorLine("Неверно заданы настройки СОМ порта устройства!");
        addTelemeryLine("Ошибка задания настроек СОМ порта устройства! [openDevice]");
        return false;
    }

    // Настройки порта
    comPort->setPortName(comPortName);
    comPort->setBaudRate(baudRate);
    comPort->setDataBits(QSerialPort::Data8);
    comPort->setParity(QSerialPort::NoParity);
    comPort->setStopBits(QSerialPort::OneStop);
    comPort->setFlowControl(QSerialPort::NoFlowControl);

    // Открываем порт
    if (!comPort->open(QIODevice::ReadWrite)) {
        addErrorLine("Ошибка открытия порта " + comPortName);
        addTelemeryLine("Ошибка открытия порта " + comPortName + " [openDevice]");
        return false;
    }

    addTelemeryLine("Устройство подключено к " + comPortName + " успешно! [openDevice]");

    return true;
}

bool ComPortDevice::closeDevice()
{
    if (comPort == nullptr){
        addErrorLine("Ошибка создания экземпляра устройства!");
        addTelemeryLine("Ошибка создания экземпляра устройства [closeDevice]");
        return false;
    }

    // Закрываем порт если он открыт
    if (comPort->isOpen()) {
        comPort->close();
    }

    addTelemeryLine("Устройство отключено от " + comPortName + " успешно! [closeDevice]");
    return true;
}

void ComPortDevice::setComPort(QString port)
{
    this->comPortName = port;
}

void ComPortDevice::setBaudRate(QSerialPort::BaudRate br)
{
    this->baudRate = br;
}

void ComPortDevice::setWorkByInterrupt(bool state)
{
    workByInterrupt = state;

    if (workByInterrupt) {
        connect(comPort, &QSerialPort::readyRead, this, &ComPortDevice::readByInterrupt);
    } else {
        disconnect(comPort, &QSerialPort::readyRead, this, &ComPortDevice::readByInterrupt);
    }
}

bool ComPortDevice::isWorkByInterrupt() const
{
    return workByInterrupt;
}
//=====================================================================

// ЗАПИСЬ ДАННЫХ В ПОРТ С ОЖИДАНИЕМ ОТВЕТА
bool ComPortDevice::read(QByteArray &dataToPort, QByteArray &pResult)
{
    if (isDeviceReady() == false) {
        addTelemeryLine("Строка запроса: " + dataToPort + " НЕ отправлена!");
        return false;
    }

    // Если работаем по прерываниям, то их нужно отключить чтобы не накладывался ответ
    if (workByInterrupt) {
        disconnect(comPort, &QSerialPort::readyRead, this, &ComPortDevice::readByInterrupt);
    } else {
        connect(comPort, &QSerialPort::readyRead, this, &ComPortDevice::readByInterrupt);
    }

    QByteArray dataAnswer = "";

    writeToPort(dataToPort);

    // Стоим ждем ответа 500мс, если ответ пришел раньше идем дальше
    comPort->waitForReadyRead(500);
    dataAnswer.append(comPort->readAll());

    // На случай если за 500мс все данные не переданы
    while (comPort->waitForReadyRead(100)){
        dataAnswer.append(comPort->readAll());
    }

    if (dataAnswer.isEmpty()) {
        addErrorLine("Ошибка чтения данных с устройства! Данные не пришли.");
        addTelemeryLine("Ошибка чтения данных с устройства! [read]");
        return false;
    }
    pResult = dataAnswer;

    // Если работаем по прерываниям, то теперь их нужно включить
    if (workByInterrupt) {
        connect(comPort, &QSerialPort::readyRead, this, &ComPortDevice::readByInterrupt);
    } else {
        disconnect(comPort, &QSerialPort::readyRead, this, &ComPortDevice::readByInterrupt);
    }

    return true;
}

// ЗАПИСЬ ДАННЫХ В ПОРТ БЕЗ ОЖИДАНИЯ ОТВЕТА
bool ComPortDevice::write(QByteArray &dataToPort)
{
    if (isDeviceReady() == false) return false;

    comPort->write(dataToPort);
    comPort->waitForBytesWritten(200);
    return true;
}

bool ComPortDevice::writeToPort(QByteArray &sendString)
{
    // Запись данных в порт
    if (!write(sendString)){
        addTelemeryLine("Строка запроса: " + sendString + " НЕ отправлена!");
        return false;
    }

    addTelemeryLine("Строка запроса: " + sendString + " отправлена!");
    return true;
}

// ЧТЕНИЕ ДАННЫХ ПО ПРЕРЫВАНИЮ
bool ComPortDevice::readByInterrupt()
{
/* Для чтения из порта по прерыванию нужно установить connect (в setReadByInterrupt)
 * Как только данные прийдут в порт это connect нужно разорвать и принять все данные
 * После этого connect восстановить
 * Принятые данные будут высланы сигналом void dataInterruptReady(QString data);
 * ПРИМЕЧАНИЕ: Не стоит использовать вместе с принятием данных по запросу
 * может наложиться два события - прерывание и отет на запрос
*/

    if (isDeviceReady() == false) return false;

    // Данные со сканере передаются частями поэтомуу отключаем connect после приема первого байта
    // и просто ждем пока все данные не передадутся
    disconnect(comPort, &QSerialPort::readyRead, this, &ComPortDevice::readByInterrupt);
    QString dataString;
    dataString.append(comPort->readAll());

    while (comPort->waitForReadyRead(50)){
        dataString.append(comPort->readAll());
    }

    emit dataInterruptReady(dataString); // сигнал о том что все данные приняты

    // делаем новое соединение для приема
    connect(comPort, &QSerialPort::readyRead, this, &ComPortDevice::readByInterrupt);

    return true;
}

bool ComPortDevice::isDeviceReady()
{
    if (comPort == nullptr){
        addErrorLine("Ошибка создания экземпляра устройства!");
        addTelemeryLine("Ошибка создания экземпляра устройства [isDeviceReady]");
        return false;
    }

    if (!comPort->isOpen()){
        addErrorLine("Ошибка записи в устройство! Устройство закрыто.");
        addTelemeryLine("Ошибка записи в устройство! [isDeviceReady]");
        return false;
    }
    return true;
}
