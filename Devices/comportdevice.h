#ifndef COMPORTDEVICE_H
#define COMPORTDEVICE_H

/*
 * Для работы с классом нужно при создании экземпляра класса
 * задать имя порта и его скорость через конструктор или setComPort и setBaudRate.
 * По умолчанию порт работает в режиме по запросам
 * Если нужно задать работу по прерыванию (так работает сканер штрих-кода и мультиметр В7-64)
 * тогда нужно установить setReadByInterrupt(true)
 * Теоретически можно работать в обоих режимах одновременно
 * Для этого при запросе отключаются прерывания и включаются снова после приема ответа на запрос
*/


#include <QObject>
#include <QSerialPort>
#include <QVector>
#include <QDebug>
#include "device.h"

class ComPortDevice : public Device
{
    Q_OBJECT
public:
    // если не известно - ни порт, ни скорость порта - нужно искать устройство
    ComPortDevice(QString deviceName, QObject *parent = nullptr);
    // порт известен, но неизвестна скорость передачи - нужно определять скорость передачи
    ComPortDevice(QString deviceName, QString port,  QObject *parent = nullptr);
    // порт и скорость передачи известна - конкретное устройство
    ComPortDevice(QString deviceName, QString port, QSerialPort::BaudRate br, QObject *parent = nullptr);
    virtual ~ComPortDevice();

    // Device interface
public:
    bool openDevice() override;
    bool closeDevice() override;

    void setComPort(QString port);
    void setBaudRate(QSerialPort::BaudRate br);
    void setWorkByInterrupt(bool state);
    bool isWorkByInterrupt() const;

    bool read(QByteArray &dataToPort,  QByteArray &pResult);
    bool write(QByteArray &dataToPort);
    bool writeToPort(QByteArray &sendString);

signals:
    void dataInterruptReady(QString s);

private:
    QSerialPort *comPort = nullptr;
    QString comPortName;
    QSerialPort::BaudRate baudRate = QSerialPort::UnknownBaud;
    bool workByInterrupt = false;

    bool readByInterrupt();
    bool isDeviceReady();

};

#endif // COMPORTDEVICE_H
