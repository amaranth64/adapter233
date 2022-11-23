#ifndef GDS71072_H
#define GDS71072_H

#include <QObject>
#include <QDebug>
#include "windows.h"

#include "Devices/comportdevice.h"

class GDS71072 : public ComPortDevice
{
    Q_OBJECT
public:
    // используем конструктор с заданием порта и скорости
    GDS71072(QString deviceName, QString port, QSerialPort::BaudRate br, QObject *parent = nullptr);
    ~GDS71072();

    enum GDS71072Channel {
            CH1, CH2
        }; Q_ENUM(GDS71072Channel)

    bool autoset();

    bool getIDN(QString &idn);
    bool getSignal(GDS71072Channel channel, QVector<double> &values);
    bool getFrequency(GDS71072Channel channel, double &freq);
    bool getAmplitude(GDS71072Channel channel, double &ampl);
    bool getPeriod(GDS71072Channel channel, double &per);

private:

    bool setSourceChannel(GDS71072::GDS71072Channel channel);
};

#endif // GDS71072_H
