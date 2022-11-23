#ifndef TESTPROCEDURE6B5429503_H
#define TESTPROCEDURE6B5429503_H

#include <QObject>
#include <QDebug>

#include "common/enumTypeData.h"
#include "common/commondata.h"
#include "Methods/Adapters/adapter6b5429233.h"


class TestProcedure6B5429503 : public Adapter6B5429233
{
    Q_OBJECT
public:
    TestProcedure6B5429503(QObject *parent = nullptr);
    ~TestProcedure6B5429503();

    // CommonMethod interface
public:
    void start();
};

#endif // TESTPROCEDURE6B5429503_H
