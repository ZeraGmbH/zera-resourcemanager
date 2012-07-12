#ifdef CONSOLE_ONLY
#include <QtCore/QCoreApplication>
#else
#include <QApplication>
#endif
#include "server/serverinterface.h"
#include "resourcemanager.h"

//debug
#include "resource/resourceobject.h"

#include <QTableView>

int main(int argc, char* argv[])
{
#ifdef CONSOLE_ONLY
  QCoreApplication a(argc, argv);
#else
  QApplication a(argc,argv);
#endif

  ResourceManager* rManager=0;
  rManager=ResourceManager::getInstance();
  Server::ServerInterface* sInterface=0;
  sInterface=Server::ServerInterface::getInstance();

#ifndef CONSOLE_ONLY
  QTableView view;
  view.setModel(rManager->getModel());
  view.setGeometry(view.x(),view.y(), 1024, 576);
  view.show();
  view.setSelectionBehavior(QAbstractItemView::SelectRows);
#endif

  //TEST
  rManager->newResource(new Resource::ResourceObject("Res1",0,"A brief description",0));
  rManager->newResource(new Resource::ResourceObject("Res2",0,"A somewhat longer description than what Res1 has as description, isn't it?",256*256*256*256-1));
  //
  view.resizeColumnsToContents();
  return a.exec();
}
