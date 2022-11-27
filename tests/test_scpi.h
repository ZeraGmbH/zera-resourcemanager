#ifndef TEST_SCPI_H
#define TEST_SCPI_H

#include <QObject>

class test_scpi : public QObject
{
    Q_OBJECT
private slots:
    void addValidResource();
    void addInvalidParamCount();
    void addInvalidPort();
    void invalidScpi();
};

#endif // TEST_SCPI_H
