#ifndef FTDIADAPTERDEVICE_H
#define FTDIADAPTERDEVICE_H

#include <QObject>
#include <QDebug>
#include "qbitarray.h"

#include "Devices/comportdevice.h"

class FTDIAdapterDevice : public ComPortDevice
{
    Q_OBJECT
public:
    FTDIAdapterDevice(QString deviceName, QString port, QSerialPort::BaudRate br, QObject *parent = nullptr);
    ~FTDIAdapterDevice();

    bool checkConnection();

    class Port{
    public:
        Port(QString name);
        ~Port();

        QString getName() const;
        void set(quint8 bit, bool state);
        void set(quint8 value);
        bool get(quint8 bit);
        void clear();
        QString toString() const;

        static quint8 bitsArrayToByte(QBitArray bA);
        static void byteToQBitArray(quint8 num, QBitArray &data);

    private:
        QString portName;
        QBitArray baPort;// = *new QBitArray(8);
    };

    bool setPort(const Port &port);
    bool getPort(FTDIAdapterDevice::Port &port);

};

#endif // FTDIADAPTERDEVICE_H
