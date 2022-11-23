#include "sdgwavegenerator.h"

SDGWaveGenerator::SDGWaveGenerator(QString deviceName, QObject *parent):Device(deviceName, parent)
{
    qDebug() << "create " + getDeviceName();
}

SDGWaveGenerator::~SDGWaveGenerator()
{
    clearDevice();
    closeDevice();
    qDebug() << "delete " + getDeviceName();
}

bool SDGWaveGenerator::openDevice()
{
    ViSession defaultRM;
    ViSession instr;
    ViUInt32 numInstrs;
    ViFindList findList;
    ViStatus status;
    char instrResourceString[VI_FIND_BUFLEN];
    unsigned char buffer[100];

    // Получаем указатель на Resource Manager defaultRM
    status=viOpenDefaultRM (&defaultRM);
    if (status<VI_SUCCESS)
    {
        addErrorLine("Ошибка открытия VISA Resource Manager!");
        return false;
    }

    // получаем список всех устройств VISA
    status = viFindRsrc (defaultRM, "USB?*INSTR", &findList, &numInstrs, instrResourceString);
    if (status<VI_SUCCESS)
    {
        addErrorLine("Ошибка поиска устройств VISA");
        viClose (defaultRM);
        return false;
    }

    // Ищем в списке генератор
    for (quint16 i = 0; i < numInstrs; i++)
    {
        if (i > 0) viFindNext (findList, instrResourceString);
        status = viOpen (defaultRM, instrResourceString, VI_NULL, VI_NULL, &instr);
        if (status<VI_SUCCESS)
        {
            addErrorLine("Ошибка открытия устройства " + QString::number(i + 1));
            continue;
        }

        QString str = "*IDN?\n";
        std::string stdStr = str.toStdString();
        status = viPrintf (instr, stdStr.c_str());
        if (status<VI_SUCCESS)
        {
            addErrorLine("Ошибка связи с устройством " + QString::number(i + 1));
            status = viClose (instr);
            continue;
        }

        Sleep(500);
        QString strAnswer;

        status = viScanf(instr, "%t", buffer);
        if (status<VI_SUCCESS){
            addErrorLine("Ошибка отклика устройства " + QString::number(i + 1));
            return false;
        }
        else{
            for(quint8 i = 0; i < 100; i++) {
                strAnswer.append(buffer[i]);
                if (buffer[i] == 10) break;
            }

            if (strAnswer.contains("SDG")) {
                devSession = instr;
                addTelemeryLine("Найдено устройство " + strAnswer);
                deviceOpened = true;
                return true;
            } else {
                viClose(instr); // ДОБАВИЛ
            }
        }
    }

    return false;
}

bool SDGWaveGenerator::closeDevice()
{
    if (deviceOpened){
        viClose(devSession);
        deviceOpened = false;
        return true;
    }
    return false;
}

//=======================================================

bool SDGWaveGenerator::clearDevice()
{
    QByteArray sendString = "*CLS\n";
    addTelemeryLine("Строка запроса: " + sendString + " очистки устройства [clearDevice]");

    return write(sendString);
}

bool SDGWaveGenerator::setSignalForm(SDGWaveGenerator::SDGSignalForm form, SDGWaveGenerator::SDGChannel ch)
{
    QString sendString;

    switch (ch) {
    case CH1:
        sendString = "C1: BSWV WVTP, ";
        break;
    case CH2:
        sendString = "C2: BSWV WVTP, ";
        break;
    default:
        addErrorLine("Ошибка выбора канала");
        return false;
        break;
    }

    switch (form) {
    case SINE:
        sendString = sendString + "SINE\n";
        break;
    case SQUARE:
        sendString = sendString + "SQUARE\n";
        break;
    case RAMP:
        sendString = sendString + "RAMP\n";
        break;
    case PULSE:
        sendString = sendString + "PULSE\n";
        break;
    case NOISE:
        sendString = sendString + "NOISE\n";
        break;
    case ARB:
        sendString = sendString + "ARB\n";
        break;
    case DC:
        sendString = sendString + "DC\n";
        break;
    default:
        addErrorLine("Ошибка выбора канала");
        return false;
        break;
    }

    addTelemeryLine("Строка запроса: " + sendString + " установки формы сигнала [setSignalForm]");

    return write(sendString);
}

bool SDGWaveGenerator::setSignalAmp(SDGWaveGenerator::SDGChannel ch, double amp)
{
    QString sendString;

    switch (ch) {
    case CH1:
        sendString = "C1: BSWV AMP, ";
        break;
    case CH2:
        sendString = "C2: BSWV AMP, ";
        break;
    default:
        addErrorLine("Ошибка выбора канала");
        return false;
        break;
    }

    sendString = sendString + QString::number(amp, 'f', 3) + "\n";

    return write(sendString);
}

bool SDGWaveGenerator::setSignalFrq(SDGWaveGenerator::SDGChannel ch, quint16 frq)
{
    QString sendString;

    switch (ch) {
    case CH1:
        sendString = "C1: BSWV FRQ, ";
        break;
    case CH2:
        sendString = "C2: BSWV FRQ, ";
        break;
    default:
        addErrorLine("Ошибка выбора канала");
        return false;
        break;
    }

    sendString = sendString + QString::number(frq) + "\n";

    return write(sendString);
}

bool SDGWaveGenerator::setOutput(SDGWaveGenerator::SDGState state, SDGWaveGenerator::SDGChannel ch)
{
    QString sendString;

    switch (ch) {
    case CH1:
        sendString = "C1: OUTP ";
        break;
    case CH2:
        sendString = "C2: OUTP ";
        break;
    default:
        addErrorLine("Ошибка выбора канала");
        return false;
        break;
    }

    switch (state) {
    case ON:
        sendString = sendString + "ON\n";
        break;
    case OFF:
        sendString = sendString + "OFF\n";
        break;
    default:
        addErrorLine("Ошибка выбора состояния выхода");
        return false;
        break;
    }

    return write(sendString);
}

//=======================================================

bool SDGWaveGenerator::write(QString data)
{
    if (deviceOpened){
        addErrorLine("Устройство не открыто!");
        return false;
    }

    ViStatus status;

    std::string stdStr = data.toStdString();
    status = viPrintf (devSession, stdStr.c_str());

    if (status<VI_SUCCESS)
    {
        addErrorLine("Ошибка связи с устройством SDG VISA" );
        addTelemeryLine("Запрос НЕ отправлен!");
        return false;
    }
    addTelemeryLine("Запрос отправлен успешно!");
    return true;

}

bool SDGWaveGenerator::read(QString data, QString &pResult)
{
    ViStatus status;
    unsigned char buffer[100];

    std::string stdStr = data.toStdString();
    status = viPrintf (devSession, stdStr.c_str());

    if (status<VI_SUCCESS)
    {
        addErrorLine("Ошибка связи с устройством SDG VISA" );
        return false;
    }

    Sleep(1000);

    status = viScanf(devSession, "%t", buffer);
    pResult.clear();

    if (status<VI_SUCCESS){
        addErrorLine("Ошибка отклика устройства SDG VISA" );
        addTelemeryLine("ответ НЕ получен!");
        return false;
    }
    else{
         for(quint8 i=0; i < 100; i++) {
             pResult.append(buffer[i]);
             if (buffer[i] == 10) break;
         }
         addTelemeryLine("ответ устройства " + pResult);
         return true;
    }
}

