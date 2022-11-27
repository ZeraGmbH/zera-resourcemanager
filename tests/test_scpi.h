#ifndef TEST_SCPI_H
#define TEST_SCPI_H

#include "resourcemanager.h"
#include "resman_scpiinterface.h"
#include "clientmultitontest.h"
#include <QObject>

class test_scpi : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void addValidResource();
    void addValidResourceAndCatalog();
    void addValidResourceAndCatalogType();
    void addValidResourceTwice();
    void addValidResourceAndRemove();
    void addValidResourceAndQuery();
    void addNestedAndQuery(); // TODO: sub nodes can be entered but will not be found anymore
    void addNestedAsParamAndQuery(); // TODO: sub nodes can be entered but will not be found anymore
    void addValidResourceAndSetAndQuery();
    void addValidResourceAndSetAndFreeAndQuery(); // TODO: free ignores additional parameters

    void addInvalidParamCount();
    void addValidAndRemoveInvalidSub();
    void addInvalidPort();
    void invalidScpi();
    void freeOnEmpty();
    void setMoreThanAvailable();
    void freeUnset();
    void freeMoreThanSet(); // TODO: free ignores additional parameters

private:
    void sendScpi(const char *cmd);
    ResourceManager *m_resMan;
    SCPI::SCPIInterface *m_scpiInterface;
    ClientMultitonTest *m_client;
};

#endif // TEST_SCPI_H
