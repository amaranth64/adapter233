#include "device.h"

Device::Device(QString deviceName, QObject *parent) : QObject(parent), deviceName(deviceName)
{
    qDebug() << "create Device of " + getDeviceName();
}

Device::~Device()
{
    qDebug() << "delete Device of " + getDeviceName();
}

QString Device::getDeviceName() const
{
    return deviceName;
}

void Device::addTelemeryLine(QString text)
{
    emit telemetryChanged(deviceName + ": " + text);
}

void Device::addErrorLine(QString text)
{
    emit errorChanged("ERROR: " + deviceName + " - " + text);
}

