#ifndef COMMONDATA_H
#define COMMONDATA_H

#include <QObject>
#include <QMutex>
#include <QMap>
#include <QDate>
#include "enumtypedata.h"

template <class T>
class Singleton {
public:
  static T& instance() {
    static T instance;
    return instance;
  }
private:
  Singleton();
  ~Singleton();
  Singleton(const Singleton &);
  Singleton& operator=(const Singleton &);
};


class CommonData : public QObject
{
    Q_OBJECT
public:
    explicit CommonData(QObject *parent = nullptr);
    ~CommonData();

    void clearData();


    QString getAdapterName() const;
    void setAdapterName(QString value);

    QString getWorkItemType() const;
    void setWorkItem(QString value);

    QString getPunkt() const;
    void setPunkt(QString value);

    QString getResult() const;
    void setResult(QString value);

    UserData getUserData() const;
    void setUserData(UserData value);

    bool isSaveToBase() const;
    void setSaveToBase(bool value);

    QString getWorkItemNumber() const;
    void setWorkItemNumber(QString value);

    QString getAdapterPort() const;
    void setAdapterPort(QString value);

    QMap<QString, QString> getWorkData() const;
    QString getWorkDataValue(QString key) const;
    void addWorkData(QString key, QString value);
    void clearWorkData();



    QString getWorkItemData() const;
    void setWorkItemData(const QString value);

signals:
    void dataChanged();

private:
    QString sAdapter = "";
    QString sAdapterPort = "";
    QString sWorkItem = "";
    QString sWorkItemNumber = "";
    QString sPunkt = "";
    UserData user;
    QString sResult = "";
    QString itemDate;

    QMap<QString, QString> workData;

    bool saveToBase = false;


};

#define dataASK Singleton<CommonData>::instance()

#endif // COMMONDATA_H
