#ifndef PSP405_H
#define PSP405_H

#include <QObject>
#include <QDebug>

#include "Devices/comportdevice.h"

class PSP405 : public ComPortDevice
{
    Q_OBJECT
public:
    // используем конструктор с заданием порта и скорости
    PSP405(QString deviceName, QString port, QSerialPort::BaudRate br, QObject *parent = nullptr);
    ~PSP405();

    enum PSP405State {
        ON,
        OFF
    };
    Q_ENUM(PSP405State)

    bool setVoltage(double V);
    bool setVoltageLimit(double Ulim);
    bool setCurrentLimit(double Ilim);

    bool setOutput(PSP405State state);

    bool readVoltage(double &voltage);
    bool readCurrent(double &current);

};

#endif // PSP405_H
