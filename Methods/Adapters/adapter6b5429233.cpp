#include "adapter6b5429233.h"

Adapter6B5429233::Adapter6B5429233(QObject *parent):CommonMethod(parent)
{
    qDebug() << "create adapter 6B5.429.233";

    // создаем порты
    portA = new FTDIAdapterDevice::Port("PortA");
    portB = new FTDIAdapterDevice::Port("PortB");
    portC = new FTDIAdapterDevice::Port("PortC");
    portD = new FTDIAdapterDevice::Port("PortD");
    portE = new FTDIAdapterDevice::Port("PortE");
    portF = new FTDIAdapterDevice::Port("PortF");
    portH = new FTDIAdapterDevice::Port("PortH");
    portG = new FTDIAdapterDevice::Port("PortG");
}

Adapter6B5429233::~Adapter6B5429233()
{
    delete portA;
    delete portB;
    delete portC;
    delete portD;
    delete portE;
    delete portF;
    delete portH;
    delete portG;

    qDebug() << "delete adapter 6B5.429.233";
}

bool Adapter6B5429233::init()
{
    bool result = true;

    result = devicePCI1727->openDevice(); // ADVANTECH
    result = result & deviceAdapter->openDevice(); // ADAPTER
    result = result & deviceGDM78341_1->openDevice(); // MULTIMETR 1
    result = result & devicePSP405->openDevice(); // POWER 27
    result = result & deviceGPD72303->openDevice(); // POWER 12
    result = result & deviceGDS71072->openDevice(); // OSCIL
    result = result & deviceGenerator->openDevice(); // GENERATOR

    if (!result){
        addProcessLine("Ошибка инициализации устройств!");
        return false;
    };

    return result;
}

bool Adapter6B5429233::startSettings()
{
    addTelemetryLine("Инициализация оборудования");
    if (!init()){
        stop();
        emit showMessageBox(AppMessageType::ERRORCODE, "Ошибка инициализации оборудования!");
        return false;
    }

    if (!isRunning()) return false;

    //===================================================================================================

    addTelemetryLine("Мультиметр в режим DC");
    deviceGDM78341_1->setRegime(GDM78341::DC);

    if (!isRunning()) return false;

    //===================================================================================================

    addTelemetryLine("Включение питания адаптера");
    powerAdapter(true);
    wait(2000);

    if (!isRunning()) return false;

    //===================================================================================================

    addTelemetryLine("Проверка связи с адаптером");
    if (!deviceAdapter->checkConnection()){
        emit showMessageBox(AppMessageType::ERRORCODE, "Ошибка инициализации оборудования!");
        return false;
    }

    if (!isRunning()) return false;

    //===================================================================================================

    addTelemetryLine("Включение питания");
    if (!powerWorkItem27V(true)) {
        emit showMessageBox(AppMessageType::ERRORCODE, "Не удалось выставить напряжение 27В!");
        return false;
    }

    if (!isRunning()) return false;

    addTelemetryLine("Включение питания");
    if (!powerWorkItem12V(true)) {
        emit showMessageBox(AppMessageType::ERRORCODE, "Не удалось выставить напряжение 27В!");
        return false;
    }

    if (!isRunning()) return false;

    //===================================================================================================

    if (!switchToggle(S1,true)) {
        emit showMessageBox(AppMessageType::ERRORCODE, "Ошибка включения тумблера S1 (питание +27B)");
        return false;
    }

    if (!switchToggle(S3,true)) {
        emit showMessageBox(AppMessageType::ERRORCODE, "Ошибка включения тумблера S1 (питание 12B)");
        return false;
    }

    if (!isRunning()) return false;

    //===================================================================================================

    addTelemetryLine("Подключение генератора");
    if (!deviceGenerator->setSignalForm(SDGWaveGenerator::SQUARE,SDGWaveGenerator::CH1)){
        emit showMessageBox(AppMessageType::ERRORCODE, "Ошибка инициализации оборудования!");
        return false;
    }

    if (!deviceGenerator->setOutput(SDGWaveGenerator::ON, SDGWaveGenerator::CH1)){
        emit showMessageBox(AppMessageType::ERRORCODE, "Ошибка инициализации оборудования!");
        return false;
    }

    if (!deviceGenerator->setSignalAmp(SDGWaveGenerator::CH1,0.01)){
        emit showMessageBox(AppMessageType::ERRORCODE, "Ошибка инициализации оборудования!");
        return false;
    }

    if (!deviceGenerator->setSignalFrq(SDGWaveGenerator::CH1, 0)){
        emit showMessageBox(AppMessageType::ERRORCODE, "Ошибка инициализации оборудования!");
        return false;
    }
    if (!isRunning()) return false;

    //===================================================================================================

//    if (!deviceGDS71072->autoset()){
//        emit showMessageBox(AppMessageType::ERRORCODE, "Ошибка инициализации оборудования!");
//        return false;
//    }

//    if (!isRunning()) return false;

    //===================================================================================================

    return true;
}

//============================================================================================================

bool Adapter6B5429233::switchS4(QString positionS4)
{
    portA->set(6, false);
    portA->set(5, false);

    if (positionS4 == "I")   {portA->set(6, true);}
    if (positionS4 == "II")  {portA->set(6, true); portA->set(5, true);}
    // позиция "0" устанавливается в любых других случаях

    addProcessLine("S4 в положение " + positionS4);

    if (!deviceAdapter->setPort(*portA)){
        addProcessLine("Ошибка установки тумблера!");
        return false;
    };

    return true;
}

bool Adapter6B5429233::switchS10(QString positionS10)
{
    portH->set(0, false);
    portH->set(1, false);
    portH->set(2, false);

    if (positionS10 == "t2")  {portH->set(0, true);}
    if (positionS10 == "Ф")   {portH->set(1, true);}
    if (positionS10 == "ИМ6") {portH->set(0, true); portH->set(0, true);}
    if (positionS10 == "СР")  {portH->set(2, true);}
    if (positionS10 == "ИМ")  {portH->set(0, true); portH->set(2, true);}
    // позиция "t1" устанавливается в любых других случаях

    addProcessLine("S10 в положение " + positionS10);

    if (!deviceAdapter->setPort(*portH)){
        addProcessLine("Ошибка установки тумблера!");
        return false;
    };

    return true;
}

bool Adapter6B5429233::switchToggle(Adapter6B5429233::SwitchType switcher, bool state)
{
    QString addState;
    if (state) addState = "ON"; else addState = "OFF";

    switch (switcher) {

    case S1:
        portA->set(7, state);
        addProcessLine("тумблер S1 = " + addState);
    break;

    case S3:
        portA->set(7, state);
        addProcessLine( "тумблер S3 = " + addState);
    break;

    case S5:
        portA->set(4, state);
        addProcessLine( "тумблер S5 = " + addState);
    break;

    case S6:
        portA->set(3, state);
        addProcessLine( "тумблер S6 = " + addState);
    break;

    case S7:
        portA->set(2, state);
        addProcessLine( "тумблер S7 = " + addState);
    break;

    case S8:
        portA->set(1, state);
        addProcessLine( "тумблер S8 = " + addState);
    break;

    case S11:
        portA->set(0, state);
        addProcessLine( "тумблер S11 = " + addState);
    break;

    default:

        break;
    }

    if (!deviceAdapter->setPort(*portA)){
        addProcessLine("Ошибка установки тумблера!");
        return false;
    };

    return true;

}

//============================================================================================================

bool Adapter6B5429233::controlLampH1()
{
    deviceAdapter->getPort(*portE);

    if (portE->get(7) == 0) {// лампа горит
        return true;
    }
    return false;
}

bool Adapter6B5429233::controlLampH2()
{
    deviceAdapter->getPort(*portE);

    if (portE->get(6) == 0) {// лампа горит
        return true;
    }
    return false;
}

bool Adapter6B5429233::controlLampH3()
{
    deviceAdapter->getPort(*portE);

    if (portE->get(5) == 0) {// лампа горит
        return true;
    }
    return false;
}

//============================================================================================================

bool Adapter6B5429233::setMeasureChannel(Adapter6B5429233::MeasureChannel channel)
{
    QString str;

    switch (channel) {
    case V27:
        portF->set(7, true);
        portF->set(6, false);
        portF->set(5, false);
        portF->set(4, false);
        portF->set(3, false);
        portF->set(2, false);
        portF->set(1, false);
        portF->set(0, false);

        portC->set(7, false);
        portC->set(6, false);
        portC->set(5, false);
        portC->set(1, false);

        str = "КОММУТАЦИЯ: канал V27";
        break;

    case V12p:
        portF->set(7, false);
        portF->set(6, false);
        portF->set(5, false);
        portF->set(4, false);
        portF->set(3, false);
        portF->set(2, false);
        portF->set(1, true);
        portF->set(0, false);

        portC->set(7, false);
        portC->set(6, false);
        portC->set(5, false);
        portC->set(1, false);

        str = "КОММУТАЦИЯ: канал V12p";
        break;

    case V12m:
        portF->set(7, false);
        portF->set(6, true);
        portF->set(5, false);
        portF->set(4, false);
        portF->set(3, false);
        portF->set(2, false);
        portF->set(1, true);
        portF->set(0, false);

        portC->set(7, false);
        portC->set(6, false);
        portC->set(5, false);
        portC->set(1, false);

        str = "КОММУТАЦИЯ: канал V12m";
        break;

    case X1:
        portF->set(7, false);
        portF->set(6, false);
        portF->set(5, false);
        portF->set(4, false);
        portF->set(3, false);
        portF->set(2, false);
        portF->set(1, false);
        portF->set(0, false);

        portC->set(7, false);
        portC->set(6, true);
        portC->set(5, false);
        portC->set(1, false);

        str = "КОММУТАЦИЯ: канал X1";
        break;

    case X3:
        portF->set(7, false);
        portF->set(6, false);
        portF->set(5, true);
        portF->set(4, false);
        portF->set(3, false);
        portF->set(2, false);
        portF->set(1, false);
        portF->set(0, false);

        portC->set(7, false);
        portC->set(6, true);
        portC->set(5, false);
        portC->set(1, false);

        str = "КОММУТАЦИЯ: канал X3";
        break;

    case X4:
        portF->set(7, false);
        portF->set(6, false);
        portF->set(5, false);
        portF->set(4, false);
        portF->set(3, false);
        portF->set(2, false);
        portF->set(1, false);
        portF->set(0, true);

        portC->set(7, false);
        portC->set(6, true);
        portC->set(5, false);
        portC->set(1, false);

        str = "КОММУТАЦИЯ: канал X4";
        break;

    case X5:
        portF->set(7, false);
        portF->set(6, false);
        portF->set(5, false);
        portF->set(4, true);
        portF->set(3, false);
        portF->set(2, false);
        portF->set(1, false);
        portF->set(0, true);

        portC->set(7, false);
        portC->set(6, true);
        portC->set(5, false);
        portC->set(1, false);

        str = "КОММУТАЦИЯ: канал X5";
        break;

    case X18:
        portF->set(7, false);
        portF->set(6, false);
        portF->set(5, false);
        portF->set(4, false);
        portF->set(3, false);
        portF->set(2, false);
        portF->set(1, false);
        portF->set(0, false);

        portC->set(7, false);
        portC->set(6, false);
        portC->set(5, false);
        portC->set(1, true);

        str = "КОММУТАЦИЯ: канал X18";
        break;

    case P1:
        portF->set(7, false);
        portF->set(6, false);
        portF->set(5, false);
        portF->set(4, false);
        portF->set(3, true);
        portF->set(2, false);
        portF->set(1, false);
        portF->set(0, false);

        portC->set(7, false);
        portC->set(6, false);
        portC->set(5, false);
        portC->set(1, true);

        str = "КОММУТАЦИЯ: канал P1";
        break;

    case V9:
        portF->set(7, false);
        portF->set(6, false);
        portF->set(5, false);
        portF->set(4, false);
        portF->set(3, false);
        portF->set(2, false);
        portF->set(1, false);
        portF->set(0, false);

        portC->set(7, true);
        portC->set(6, false);
        portC->set(5, false);
        portC->set(1, true);

        str = "КОММУТАЦИЯ: канал V9";
        break;

    case X7:
        portF->set(7, false);
        portF->set(6, false);
        portF->set(5, false);
        portF->set(4, false);
        portF->set(3, false);
        portF->set(2, true);
        portF->set(1, false);
        portF->set(0, false);

        portC->set(7, true);
        portC->set(6, false);
        portC->set(5, false);
        portC->set(1, true);

        str = "КОММУТАЦИЯ: канал X7";
        break;

    default:
        str = "КОММУТАЦИЯ: ошибка задания канала";
    break;
    }

    addProcessLine(str);

    bool result;

    result = deviceAdapter->setPort(*portC);
    result = result & deviceAdapter->setPort(*portF);

    if (!result){
        addProcessLine("Ошибка коммутации мультиметра!");
        return false;
    };

    return true;
}

bool Adapter6B5429233::measureValue(Adapter6B5429233::MeasureChannel channel, double &result)
{
    double coeff = 1; // коэффициенты для делителей напряжения
    QString str, strAdd;

    if(!setMeasureChannel(channel)){
        addProcessLine("ОШИБКА КОММУТАЦИИ!");
        return false;
    };

    wait(2000);

    if (!isRunning()) {
        return false;
    }

    switch (channel) {
    case V27:

        str = "ИЗМЕРЕНИЕ: канал V27 %1 V";
        break;

    case V12p:

        str = "ИЗМЕРЕНИЕ: канал V12p %1 V";
        break;

    case V12m:

        str = "ИЗМЕРЕНИЕ: канал V12m %1 V";
        break;

    case X1:

        str = "ИЗМЕРЕНИЕ: канал X1 %1 V";
        break;

    case X3:
        coeff = 1;
        str = "ИЗМЕРЕНИЕ: канал X3 %1 V";
        break;

    case X4:

        str = "ИЗМЕРЕНИЕ: канал X4 %1 V";
        break;

    case X5:

        str = "ИЗМЕРЕНИЕ: канал X5 %1 V";
        break;

    case X18:

        str = "ИЗМЕРЕНИЕ: канал X18 %1 V";
        break;

    case P1:

        str = "ИЗМЕРЕНИЕ: канал P1 %1 V";
        break;

    case V9:

        str = "ИЗМЕРЕНИЕ: канал V9 %1 V";
        break;

    case X7:

        str = "ИЗМЕРЕНИЕ: канал X7 %1 V";
        break;

    default:
        str = "ИЗМЕРЕНИЕ: ошибка задания канала";
        addProcessLine(str);
    break;
    }

    double dataV = 0.0;
    if (!deviceGDM78341_1->readValue(dataV)){
        addProcessLine("ОШИБКА ИЗМЕРЕНИЯ!");
        return result;
    };

    result = coeff * dataV;
    addProcessLine(str.arg(result));

    return result;
}
