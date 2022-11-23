#ifndef TESTPROCEDURE6B5429426_H
#define TESTPROCEDURE6B5429426_H

#include <QObject>
#include <QDebug>

#include "common/enumTypeData.h"
#include "common/commondata.h"
#include "Methods/Adapters/adapter6b5429233.h"

class TestProcedure6B5429426 : public Adapter6B5429233
{
    Q_OBJECT
public:
    TestProcedure6B5429426(QObject *parent = nullptr);
    ~TestProcedure6B5429426();

    // CommonMethod interface
public:
    void start();

    bool checkPunkt41();
    bool checkPunkt42();
    bool checkPunkt43();
    bool checkPunkt44();
    bool checkPunkt45();
    bool checkPunkt46();

};

#endif // TESTPROCEDURE6B5429426_H
