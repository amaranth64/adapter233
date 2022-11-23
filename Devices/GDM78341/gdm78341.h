#ifndef GDM78341_H
#define GDM78341_H

#include <QObject>
#include <QDebug>

#include "Devices/comportdevice.h"

class GDM78341 : public ComPortDevice
{
    Q_OBJECT
public:
    // используем конструктор с заданием порта и скорости
    GDM78341(QString deviceName, QString port, QSerialPort::BaudRate br, QObject *parent = nullptr);
    ~GDM78341();

    enum GDM78341Regime {
            UNKNOWN,
            DC,
            AC,
            FREQ,
            PER,
            CONT
        };
        Q_ENUM(GDM78341Regime)

    bool clearDevice();
    bool idnDevice(QString &idn);
    bool setRegime(GDM78341Regime regime, QByteArray dia="");
    GDM78341Regime getRegime();
    bool readValue(double &value);

private:

    GDM78341Regime currentRegime = UNKNOWN;

};

#endif // GDM78341_H
