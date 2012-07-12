#include "resourcemanager.h"

ResourceManager::ResourceManager(QObject* parent) :
  QObject(parent)
{
  QStringList headers;
  headers.append(tr("Resource"));
  headers.append(tr("Provider"));
  headers.append(tr("Occupiers"));
  headers.append(tr("Amount"));
  headers.append(tr("Free amount"));
  headers.append(tr("Description"));

  resourceModel.setHorizontalHeaderLabels(headers);
}

ResourceManager* ResourceManager::singletonInstance=0;

ResourceManager* ResourceManager::getInstance()
{
  if(singletonInstance==0)
  {
    singletonInstance=new ResourceManager;
  }
  return singletonInstance;
}


QStandardItemModel* ResourceManager::getModel()
{
  return &resourceModel;
}


void ResourceManager::newResource(Resource::ResourceObject *resource)
{
  QModelIndex tmpIndex, currentIndex;
  QString provider, occupiers;
  resourceModel.appendRow(resource);
  tmpIndex = resourceModel.indexFromItem(resource);

  //Name of the resource
  currentIndex=tmpIndex.sibling(tmpIndex.row(),tmpIndex.column());
  resourceModel.setData(currentIndex,resource->getName(),Qt::DisplayRole);
  //Provider of the resource
  provider = resource->getProvider()!=0 ? resource->getProvider()->getName() : "";
  currentIndex=tmpIndex.sibling(tmpIndex.row(),tmpIndex.column()+1);
  resourceModel.setData(tmpIndex.sibling(tmpIndex.row(),tmpIndex.column()+1),provider,Qt::DisplayRole);
  //Occupiers of the resource
  occupiers="";
  for(int i =0; i<resource->getOccupiers().count(); i++)
  {
    occupiers.append("%1 ").arg(resource->getOccupiers().at(i)->getName());
  }
  currentIndex=tmpIndex.sibling(tmpIndex.row(),tmpIndex.column()+2);
  resourceModel.setData(currentIndex,occupiers,Qt::DisplayRole);
  //Max amount of the resource
  currentIndex=tmpIndex.sibling(tmpIndex.row(),tmpIndex.column()+3);
  resourceModel.setData(currentIndex,resource->getAmount(),Qt::DisplayRole);
  //Current available amount of the resource
  currentIndex=tmpIndex.sibling(tmpIndex.row(),tmpIndex.column()+4);
  resourceModel.setData(currentIndex,resource->getFreeAmount(),Qt::DisplayRole);
  //Description of the resource
  currentIndex=tmpIndex.sibling(tmpIndex.row(),tmpIndex.column()+5);
  resourceModel.setData(currentIndex,resource->getDescription(),Qt::DisplayRole);

  for (int i=0; i<6; i++) //disable editing of all items
  {
    QStandardItem *tmpItem;
    tmpItem=resourceModel.itemFromIndex(tmpIndex.sibling(tmpIndex.row(),tmpIndex.column()+i));
    tmpItem->setFlags(tmpItem->flags() & ~Qt::ItemIsEditable); //disable editing of the item
  }
}

void ResourceManager::deleteResource(Resource::ResourceObject *resource)
{
  QModelIndex tmpIndex;
  tmpIndex = resourceModel.indexFromItem(resource);
  if(tmpIndex.isValid())
  {
    resourceModel.removeRow(tmpIndex.row());
  }
  else
  {
    qDebug()<<"Tried to delete invalid index with name: "<<resource->getName();
  }
}

void ResourceManager::occupyResource(Resource::ResourceObject *resource, Server::Client *client, quint32 amount)
{
  if(resource->occupyResource(client,amount))
  {
    lockGranted(resource, client);
  }
  else
  {
    lockFailed(resource,client);
  }
}

void ResourceManager::freeResource(Resource::ResourceObject *resource, Server::Client *client, quint32 amount)
{
  resource->freeResource(client, amount);
}
