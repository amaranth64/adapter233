#include "commondata.h"

CommonData::CommonData(QObject *parent) : QObject(parent)
{

}

CommonData::~CommonData()
{

}

void CommonData::clearData()
{
    sAdapter.clear();
    sWorkItem.clear();
    sPunkt.clear();
    user.name.clear();
    user.userID = 0;
    sResult.clear();
    workData.clear();
}

QString CommonData::getAdapterName() const
{
    return sAdapter;
}

void CommonData::setAdapterName( QString value)
{
    sAdapter = value;
    emit dataChanged();
}

QString CommonData::getWorkItemType() const
{

    return sWorkItem;
}

void CommonData::setWorkItem(QString value)
{
    sWorkItem = value;
    emit dataChanged();
}

QString CommonData::getPunkt() const
{
    return sPunkt;
}

void CommonData::setPunkt(QString value)
{
    sPunkt = value;
    emit dataChanged();
}

QString CommonData::getResult() const
{
    return sResult;
}

void CommonData::setResult(QString value)
{
    sResult = value;
    emit dataChanged();
}

UserData CommonData::getUserData() const
{
    return user;
}

void CommonData::setUserData(UserData value)
{
    user = value;
    emit dataChanged();
}

bool CommonData::isSaveToBase() const
{
    return saveToBase;
}

void CommonData::setSaveToBase(bool value)
{
    saveToBase = value;
}

QString CommonData::getWorkItemNumber() const
{
    return sWorkItemNumber;
}

void CommonData::setWorkItemNumber(QString value)
{
    sWorkItemNumber = value;
}

QString CommonData::getAdapterPort() const
{
    return sAdapterPort;
}

void CommonData::setAdapterPort(QString value)
{
    sAdapterPort = value;
}

QString CommonData::getWorkDataValue(QString key) const
{
    return workData.value(key);
}

void CommonData::addWorkData(QString key, QString value)
{
    workData[key] = value;
}

void CommonData::clearWorkData()
{
    workData.clear();
}

QString CommonData::getWorkItemData() const
{
    return itemDate;
}

void CommonData::setWorkItemData(const QString value)
{
    itemDate = value;
}

QMap<QString, QString> CommonData::getWorkData() const
{
    return workData;
}


