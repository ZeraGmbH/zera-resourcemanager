#ifndef TEST_LEGACY_H
#define TEST_LEGACY_H

#include <QObject>

class test_legacy : public QObject
{
    Q_OBJECT
private slots:
    void initialCatalog();
};

#endif // TEST_LEGACY_H
