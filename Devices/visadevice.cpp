#include "visadevice.h"

VISADevice::VISADevice(QString deviceName, QObject *parent):Device(deviceName, parent)
{
    qDebug() << "create VISADevice of " + getDeviceName();
}

VISADevice::~VISADevice()
{
    qDebug() << "delete VISADevice of " + getDeviceName();
}
