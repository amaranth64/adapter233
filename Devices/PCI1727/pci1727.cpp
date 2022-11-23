#include "pci1727.h"

PCI1727::PCI1727(QString deviceName, QObject *parent):AdvantechCardDevice(deviceName, parent)
{
    qDebug() << "create " + getDeviceName();
}

PCI1727::~PCI1727()
{
    qDebug() << "delete "  + getDeviceName();
}

bool PCI1727::openDevice()
{
    bool result;
    result = createAOInstant();
    result = result & createDOInstant();
    isInit = true;
    return result;
}

bool PCI1727::closeDevice()
{
    return true;
}

//===========================================================================

bool PCI1727::resetDevice()
{
    if (!isInit) return false;

    for (quint8 i = 0; i < 8; i++){
        ErrorCode err = instantAoCtrl->Write(i, 0);
        if (err != Success) {
            addErrorLine("Ошибка установки AO" + QString::number(i) + "!");
            return false;
        }
    }

    instantDoCtrl->Write(0, 0);
    instantDoCtrl->Write(1, 0);

    addTelemeryLine("Устройство сброшено!");

    return true;
}

bool PCI1727::setVoltage(quint8 channel, double value)
{
    if (!isInit) return false;

    ErrorCode err = instantAoCtrl->Write(channel, value);
    if (err != Success) {
       addErrorLine("Ошибка установки напряжения на канале AO " + QString::number(channel) + "!");
        return false;
    }

    addTelemeryLine("канал AO" + QString::number(channel) + " = " + QString::number(value) + "V");

    return true;
}

bool PCI1727::setPort(quint8 port, quint8 bit, bool state)
{
    if (!isInit) return false;

    ErrorCode err = instantDoCtrl->WriteBit(port, bit, state);
    if (err != Success) {
        addErrorLine("Ошибка установки DO" + QString::number(bit) + "!");
        return false;
    }

    addTelemeryLine("bit" + QString::number(bit) + " в "  + QString::number(state));

    return true;
}
