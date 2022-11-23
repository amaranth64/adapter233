#include "commonmethod.h"

CommonMethod::CommonMethod(QObject *parent) : QObject(parent)
{
    qDebug() << "create CommonMethod";

    deviceAdapter = new FTDIAdapterDevice("Adapter", "COM1", QSerialPort::Baud56000, this);
    devicePCI1727 = new PCI1727("PCI-1727", this);
    deviceGDM78341_1 = new GDM78341("Multimeter 1", "COM9", QSerialPort::Baud9600, this);
    deviceGDM78341_2 = new GDM78341("Multimeter 2", "COM11", QSerialPort::Baud9600, this);
    devicePSP405 = new PSP405("PSP405", "COM2", QSerialPort::Baud2400, this);
    deviceGPD72303 = new GPD72303("GPD72303", "COM7", QSerialPort::Baud9600, this);
    deviceGDS71072 = new GDS71072("GDS71072", "COM8", QSerialPort::Baud9600, this);
    deviceGenerator = new SDGWaveGenerator("Generator", this);

    connect(deviceAdapter,&FTDIAdapterDevice::telemetryChanged, this, &CommonMethod::telemetryChanged);
    connect(deviceAdapter,&FTDIAdapterDevice::errorChanged, this, &CommonMethod::errorChanged);

    connect(devicePCI1727,&PCI1727::telemetryChanged, this, &CommonMethod::telemetryChanged);
    connect(devicePCI1727,&PCI1727::errorChanged, this, &CommonMethod::errorChanged);

    connect(deviceGDM78341_1,&GDM78341::telemetryChanged, this, &CommonMethod::telemetryChanged);
    connect(deviceGDM78341_1,&GDM78341::errorChanged, this, &CommonMethod::errorChanged);

    connect(deviceGDM78341_2,&GDM78341::telemetryChanged, this, &CommonMethod::telemetryChanged);
    connect(deviceGDM78341_2,&GDM78341::errorChanged, this, &CommonMethod::errorChanged);

    connect(devicePSP405,&PSP405::telemetryChanged, this, &CommonMethod::telemetryChanged);
    connect(devicePSP405,&PSP405::errorChanged, this, &CommonMethod::errorChanged);

    connect(deviceGPD72303,&GPD72303::telemetryChanged, this, &CommonMethod::telemetryChanged);
    connect(deviceGPD72303,&GPD72303::errorChanged, this, &CommonMethod::errorChanged);
\
    connect(deviceGDS71072,&GDS71072::telemetryChanged, this, &CommonMethod::telemetryChanged);
    connect(deviceGDS71072,&GDS71072::errorChanged, this, &CommonMethod::errorChanged);

    connect(deviceGenerator,&SDGWaveGenerator::telemetryChanged, this, &CommonMethod::telemetryChanged);
    connect(deviceGenerator,&SDGWaveGenerator::errorChanged, this, &CommonMethod::errorChanged);

}

CommonMethod::~CommonMethod()
{
    deviceAdapter->deleteLater();
    devicePCI1727->deleteLater();
    deviceGDM78341_1->deleteLater();
    deviceGDM78341_2->deleteLater();
    devicePSP405->deleteLater();
    deviceGPD72303->deleteLater();
    deviceGDS71072->deleteLater();
    deviceGenerator->deleteLater();

    qDebug() << "delete CommonMethod";
}

//==============================================================

void CommonMethod::stop()
{
    deviceAdapter->closeDevice();
    devicePCI1727->closeDevice();
    deviceGDM78341_1->closeDevice();
    deviceGDM78341_2->closeDevice();
    devicePSP405->closeDevice();
    deviceGPD72303->closeDevice();
    deviceGDS71072->closeDevice();
    deviceGenerator->closeDevice();

    emit finished();
    setRunning(false);
    emit progressBarChanged(100);
}

void CommonMethod::wait(quint16 mstime)
{
    mstime = static_cast<quint16>(mstime/100);
    for (quint16 i=0; i < mstime; i++){
        Sleep(100);

        if (!isRunning()) {
            qDebug() << "CommonMethod::wait exit";
            return;
        }
    }
}

void CommonMethod::addStageLine(QString punkt, QString text)
{
    StageData data;
    data.setStageData(QTime::currentTime().toString("hh:mm:ss"), punkt, text);
    emit stageChanged(data);
}

void CommonMethod::addProcessLine(QString text)
{
    emit processChanged(text);
}

void CommonMethod::addTelemetryLine(QString text)
{
    emit telemetryChanged("Методика: " + text);
}

void CommonMethod::addErrorLine(QString text)
{
    emit telemetryChanged("ERROR: Методика - " + text);
}

//===============================================================

void CommonMethod::showMessageDlg(QString caption, QString text)
{
    emit showRegulation(caption, text);
    messageDlg = true;
}

void CommonMethod::closeMessageDlg()
{
    messageDlg = false;
}

bool CommonMethod::isMessageDlgShow()
{
    return messageDlg;
}

//==============================================================

bool CommonMethod::powerAdapter(bool state)
{
    bool result = true;

    result = devicePCI1727->setPort(0, 0, state);
    result = result & devicePCI1727->setPort(0, 1, state);

    if (!result) {
        addProcessLine("ОШИБКА: Установка питания адаптера не выполнена!");
        return result;
    }

    QString str = "выключено";
    if (state) str = "включено";
    addProcessLine("Питание адаптера " + str + "!");

    return result;
}

bool CommonMethod::powerWorkItem27V(bool state)
{
    devicePSP405->setCurrentLimit(3.0);
    wait(100);

    if (state){ // включить
        addProcessLine("ВКЛЮЧЕНИЕ ПИТАНИЯ 27В");
        devicePSP405->setOutput(PSP405::ON);
        wait(500);

        if (!isRunning()) return false;

        devicePSP405->setVoltage(27.0);
        wait(500);

        if (!isRunning()) return false;
    }
    else { // выключить
        addProcessLine("ВЫКЛЮЧЕНИЕ ПИТАНИЯ 27В");
        devicePSP405->setOutput(PSP405::OFF);
        wait(500);

        if (!isRunning()) return false;

        devicePSP405->setVoltage(27.0);
        wait(500);

        if (!isRunning()) return false;
    }

    // ПРОВЕРКА НАПРЯЖЕНИЯ
    double dataV = 0.0;
    devicePSP405->readVoltage(dataV);
    addTelemetryLine("Задано напряжение на ИП " + QString::number(dataV));
    return true;
}

bool CommonMethod::powerWorkItem12V(bool state)
{
    deviceGPD72303->setCurrentLimit(2.0);
    wait(100);

    if (state){ // включить
        addProcessLine("ВКЛЮЧЕНИЕ ПИТАНИЯ 12В");

        deviceGPD72303->setVoltage(12.0);
        wait(500);

        if (!isRunning()) return false;

        deviceGPD72303->setOutput(GPD72303::ON);
        wait(500);

        if (!isRunning()) return false;
    }
    else { // выключить
        addProcessLine("ВЫКЛЮЧЕНИЕ ПИТАНИЯ 12В");

        deviceGPD72303->setVoltage(12.0);
        wait(500);

        if (!isRunning()) return false;

        deviceGPD72303->setOutput(GPD72303::OFF);
        wait(500);

        if (!isRunning()) return false;
    }

    // ПРОВЕРКА НАПРЯЖЕНИЯ
    double dataV = 0.0;
    deviceGPD72303->readVoltage(dataV);
    addTelemetryLine("Задано напряжение на ИП " + QString::number(dataV));
    return true;
}

//==============================================================

void CommonMethod::startRegulation()
{
    regulation = true;
}

void CommonMethod::stopRegulation()
{
    regulation = false;
}

bool CommonMethod::isRegulation()
{
    return regulation;
}

//==============================================================

void CommonMethod::setRunning(bool value)
{
    running = value;
}

bool CommonMethod::isRunning()
{
    return running;
}

//==============================================================

