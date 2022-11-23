#ifndef SDGWAVEGENERATOR_H
#define SDGWAVEGENERATOR_H

#include <QObject>
#include <QVector>
#include <QDebug>

#include "Devices/device.h"
#include "windows.h"

#include "visa.h"
#pragma comment(lib,"visa32.lib")

class SDGWaveGenerator : public Device
{
    Q_OBJECT
public:
    SDGWaveGenerator(QString deviceName, QObject *parent=nullptr);
    virtual ~SDGWaveGenerator();

    enum SDGSignalForm {
        SINE, SQUARE, RAMP, PULSE, NOISE, ARB ,DC
    }; Q_ENUM(SDGSignalForm)

    enum SDGState {
        ON, OFF
    }; Q_ENUM(SDGState)

    enum SDGChannel {
        CH1, CH2
    }; Q_ENUM(SDGChannel)

    bool openDevice();
    bool closeDevice();
    bool clearDevice();

    bool setSignalForm(SDGSignalForm form, SDGChannel ch);
    bool setSignalAmp(SDGChannel ch, double amp);
    bool setSignalFrq(SDGChannel ch, quint16 frq);

    bool setOutput(SDGState state, SDGChannel ch);

private:
    ViSession devSession;
    bool deviceOpened = false;

    bool write(QString data);
    bool read(QString data,  QString &pResult);

};

#endif // SDGWAVEGENERATOR_H
