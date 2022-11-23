#ifndef GPD72303_H
#define GPD72303_H

#include <QObject>
#include <QDebug>

#include "Devices/comportdevice.h"

class GPD72303 : public ComPortDevice
{
    Q_OBJECT
public:
    // используем конструктор с заданием порта и скорости
    GPD72303(QString deviceName, QString port, QSerialPort::BaudRate br, QObject *parent = nullptr);
    ~GPD72303();

    enum GPD72303State {
        ON,
        OFF
    };
    Q_ENUM(GPD72303State)

    bool setVoltage(double V);
    bool setVoltageLimit(double Ulim);
    bool setCurrentLimit(double Ilim);

    bool setOutput(GPD72303State state);

    bool readVoltage(double &voltage);
    bool readCurrent(double &current);


};

#endif // GPD72303_H
