#ifndef ADAPTER6B5429233_H
#define ADAPTER6B5429233_H

#include <QObject>
#include <QDebug>

#include "Methods/commonmethod.h"

class Adapter6B5429233 : public CommonMethod
{
    Q_OBJECT
public:
    Adapter6B5429233(QObject *parent = nullptr);
    virtual ~Adapter6B5429233();

    enum MeasureChannel{
        V27, V12p, V12m, X1, X3, X4, X5, X18, P1, V9, X7
    };

    enum SwitchType{
        S1, S3, S4, S5, S6, S7, S8, S10, S11
    };

    bool init();
    bool startSettings();

    bool switchS4(QString positionS4);
    bool switchS10(QString positionS10);
    bool switchToggle(SwitchType switcher, bool state);

    bool controlLampH1();
    bool controlLampH2();
    bool controlLampH3();

    bool setMeasureChannel(MeasureChannel channel);
    bool measureValue(MeasureChannel channel, double &result);

private:

    FTDIAdapterDevice::Port *portA;
    FTDIAdapterDevice::Port *portB;
    FTDIAdapterDevice::Port *portC;
    FTDIAdapterDevice::Port *portD;
    FTDIAdapterDevice::Port *portE;
    FTDIAdapterDevice::Port *portF;
    FTDIAdapterDevice::Port *portH;
    FTDIAdapterDevice::Port *portG;

};


#endif // ADAPTER6B5429233_H
