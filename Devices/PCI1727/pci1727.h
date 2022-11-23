#ifndef PCI1727_H
#define PCI1727_H

#include <QObject>

#include "Devices/advantechcarddevice.h"

class PCI1727 : public AdvantechCardDevice
{
    Q_OBJECT
public:
    PCI1727(QString deviceName, QObject *parent = nullptr);
    ~PCI1727();

    bool resetDevice();
    bool setVoltage(quint8 channel, double value);
    bool setPort(quint8 port, quint8 bit, bool state);

    // Device interface
public:
    bool openDevice();
    bool closeDevice();

private:
    bool isInit = false;
};

#endif // PCI1727_H
