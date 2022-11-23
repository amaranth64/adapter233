#include "advantechcarddevice.h"

AdvantechCardDevice::AdvantechCardDevice(QString deviceName, QObject *parent):Device(deviceName, parent)
{
    qDebug() << "create AdvantechCardDevice of " + getDeviceName();
}

AdvantechCardDevice::~AdvantechCardDevice()
{
    if (instantDoCtrl != nullptr) {
        instantDoCtrl->Dispose();
        instantDoCtrl = nullptr;

        qDebug() << "instantDoCtrl" << instantDoCtrl;
    }

    if (instantAoCtrl != nullptr) {
        instantAoCtrl->Dispose();
        instantAoCtrl = nullptr;

        qDebug() << "instantAoCtrl" << instantDoCtrl;
    }

    if (instantAiCtrl != nullptr) {
        instantAiCtrl->Dispose();
        instantAiCtrl = nullptr;

        qDebug() << "instantAiCtrl" << instantDoCtrl;
    }

    if (instantDiCtrl != nullptr) {
        instantDiCtrl->Dispose();
        instantDiCtrl = nullptr;

        qDebug() << "instantDiCtrl" << instantDoCtrl;
    }


    qDebug() << "delete AdvantechCardDevice of " + getDeviceName();
}

bool AdvantechCardDevice::createAOInstant()
{
    if (instantAoCtrl != nullptr){
        addErrorLine("Экземпляр instantAoCtrl уже создан");
        return false;
    }

    instantAoCtrl = InstantAoCtrl::Create();

    Array<DeviceTreeNode> *supportedDevices = instantAoCtrl->getSupportedDevices();

    if (supportedDevices->getCount() == 0){
        addErrorLine("Устройства Advantech AO не найдены!");
        return false;
    }

    std::wstring description;
    int countPCI = 0;
    for ( int i = 0; i < supportedDevices->getCount(); i++){
        description = supportedDevices->getItem(i).Description;

        if (description.find(getDeviceName().toStdWString(), 0) != std::string::npos) {
            // нашли плату и прерываем поиск
            countPCI++;
            break;
        }
    }

    // проверяем найдена ли плата
    if ((description.empty())|(countPCI == 0)){
        addErrorLine("Устройство Advantech AO " + getDeviceName() + " не найдено!");
        return false;
    }

    DeviceInformation plata(description.c_str());

    ErrorCode err;
    err = instantAoCtrl->setSelectedDevice(plata);
    if (err != Success) {
        addErrorLine("Ошибка открытия устройства в режиме AO!");
        instantAoCtrl->Dispose();
        instantAoCtrl = nullptr;
        return false;
    }

    Array<AoChannel>* channels = instantAoCtrl->getChannels();

    for (quint8 i = 0; i < channels->getCount(); i++){
        err = instantAoCtrl->Write(i, 0);
        if (err != Success) {
            addErrorLine("Ошибка установки AO" + QString::number(i));
            instantAoCtrl->Dispose();
            instantAoCtrl = nullptr;
            return false;
        };
    }

    addTelemeryLine("Экземпляр instantAoCtrl создан");


    qDebug() << "instantAoCtrl" << instantAoCtrl;

    return true;
}

bool AdvantechCardDevice::createAIInstant()
{
    if (instantAiCtrl != nullptr){
        addErrorLine("Экземпляр instantAiCtrl уже создан");
        return false;
    }

    instantAiCtrl = InstantAiCtrl::Create();

    Array<DeviceTreeNode> *supportedDevices = instantAiCtrl->getSupportedDevices();

    if (supportedDevices->getCount() == 0){
        addErrorLine("Устройства Advantech AI не найдены!");
        return false;
    }

    std::wstring description;
    int countPCI = 0;
    for ( int i = 0; i < supportedDevices->getCount(); i++){
        description = supportedDevices->getItem(i).Description;

        if (description.find(getDeviceName().toStdWString(), 0) != std::string::npos) {
            // нашли плату и прерываем поиск
            countPCI++;
            break;
        }
    }

    // проверяем найдена ли плата
    if ((description.empty())|(countPCI == 0)){
        addErrorLine("Устройство Advantech AI " + getDeviceName() + " не найдено!");
        return false;
    }

    DeviceInformation plata(description.c_str());

    ErrorCode err;
    err = instantAiCtrl->setSelectedDevice(plata);
    if (err != Success) {
        addErrorLine("Ошибка открытия устройства в режиме AI!");
        instantAiCtrl->Dispose();
        instantAiCtrl = nullptr;
        return false;
    }

    addTelemeryLine("Экземпляр instantAiCtrl создан");
    return true;
}

bool AdvantechCardDevice::createDOInstant()
{
    if (instantDoCtrl != nullptr){
        addErrorLine("Экземпляр instantDoCtrl уже создан");
        return false;
    }

    instantDoCtrl = InstantDoCtrl::Create();

    Array<DeviceTreeNode> *supportedDevices = instantDoCtrl->getSupportedDevices();



    if (supportedDevices->getCount() == 0){
        addErrorLine("Устройства Advantech DO не найдены!");
        return false;
    }

    std::wstring description;
    int countPCI = 0;
    for ( int i = 0; i < supportedDevices->getCount(); i++){

        description = supportedDevices->getItem(i).Description;

        if (description.find(getDeviceName().toStdWString(), 0) != std::string::npos) {
            // нашли плату и прерываем поиск
            countPCI++;
            break;
        }
    }

    // проверяем найдена ли плата
    if ((description.empty())|(countPCI == 0)){
        addErrorLine("Устройство Advantech DO " + getDeviceName() + " не найдено!");
        return false;
    }

    DeviceInformation plata(description.c_str());

    ErrorCode err;
    err = instantDoCtrl->setSelectedDevice(plata);
    if (err != Success) {
        addErrorLine("Ошибка открытия устройства " + getDeviceName() + "  в режиме DO!");
        instantDoCtrl->Dispose();
        instantDoCtrl = nullptr;
        return false;
    }

    Array<DioPort>* channels = instantDoCtrl->getPorts();

    for (quint8 i = 0; i < channels->getCount(); i++){
        err = instantDoCtrl->Write(i, 0);
        if (err != Success) {
            addErrorLine("ошибка установки DO");
            instantDoCtrl->Dispose();
            instantDoCtrl = nullptr;
            return false;
        };
    }

    addTelemeryLine("Экземпляр instantDoCtrl создан");

    qDebug() << "instantDoCtrl" << instantDoCtrl;
    return true;
}

bool AdvantechCardDevice::createDIInstant()
{
    if (instantDiCtrl != nullptr){
        addErrorLine("Экземпляр instantDiCtrl уже создан");
        return false;
    }

    instantDiCtrl = InstantDiCtrl::Create();

    Array<DeviceTreeNode> *supportedDevices = instantDiCtrl->getSupportedDevices();

    if (supportedDevices->getCount() == 0){
        addErrorLine("Устройства Advantech DI не найдены!");
        return false;
    }

    std::wstring description;
    int countPCI = 0;
    for ( int i = 0; i < supportedDevices->getCount(); i++){
        description = supportedDevices->getItem(i).Description;

        if (description.find(getDeviceName().toStdWString(), 0) != std::string::npos) {
            // нашли плату и прерываем поиск
            countPCI++;
            break;
        }
    }

    // проверяем найдена ли плата
    if ((description.empty())|(countPCI == 0)){
        addErrorLine("Устройства Advantech DI " + getDeviceName() + " не найдено!");
        return false;
    }

    DeviceInformation plata(description.c_str());

    ErrorCode err;
    err = instantDiCtrl->setSelectedDevice(plata);
    if (err != Success) {
        addErrorLine("Ошибка открытия устройства " + getDeviceName() + "  в режиме DI!");
        instantDiCtrl->Dispose();
        instantDiCtrl = nullptr;
        return false;
    }

    addTelemeryLine("Экземпляр instantDiCtrl создан");
    return true;
}
