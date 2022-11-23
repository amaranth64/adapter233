#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QDebug>

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QString deviceName, QObject *parent = nullptr);
    virtual ~Device();

    QString getDeviceName() const;

    virtual bool openDevice() = 0;
    virtual bool closeDevice() = 0;

protected:
    void addTelemeryLine(QString text);
    void addErrorLine(QString text);


private:
    QString deviceName;

signals:
    void telemetryChanged(QString telemetry);
    void errorChanged(QString error);

};

#endif // DEVICE_H
