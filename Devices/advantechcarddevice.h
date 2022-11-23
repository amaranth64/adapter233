#ifndef ADVANTECHCARDDEVICE_H
#define ADVANTECHCARDDEVICE_H

#include <QObject>
#include <QVector>
#include <QDebug>

#include "Devices/bdaqctrl.h"
#include "device.h"

using namespace Automation::BDaq;

/*
 * Название deviceName должно соответствовать названию платы в система
 * по этому названию осуществляется поиск этого устройства
*/
class AdvantechCardDevice : public Device
{
    Q_OBJECT
public:
    AdvantechCardDevice(QString deviceName, QObject *parent);
    virtual ~AdvantechCardDevice();

    bool createAOInstant();
    bool createAIInstant();
    bool createDOInstant();
    bool createDIInstant();

    void closePCI1727();

protected:
    InstantAoCtrl *instantAoCtrl = nullptr;
    InstantAiCtrl *instantAiCtrl = nullptr;

    InstantDoCtrl *instantDoCtrl = nullptr;
    InstantDiCtrl *instantDiCtrl = nullptr;


};

#endif // ADVANTECHCARDDEVICE_H
