#ifndef VISADEVICE_H
#define VISADEVICE_H

#include <QObject>
#include <QVector>
#include <QDebug>

#include "device.h"
class VISADevice : public Device
{
    Q_OBJECT
public:
    VISADevice(QString deviceName, QObject *parent);
    virtual ~VISADevice();
};

#endif // VISADEVICE_H
