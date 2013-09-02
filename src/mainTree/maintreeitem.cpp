#include <QStringList>

#include "mainTree/maintreeitem.h"

MainTreeItem::MainTreeItem(const QList<QVariant> &data, MainTreeItem *parent)
{
    m_parentItem = parent;
    itemData = data;
    m_povList << "";

    m_isExpandedInDockedTree = true;
    m_isExpandedInOutliner = true;
    m_isExpandedInExporter = true;

    m_type= "";
    m_status = -1;
    m_badge = "";
    m_index = QModelIndex();

}
MainTreeItem::~MainTreeItem()
{
    qDeleteAll(childItems);
}

void MainTreeItem::appendChild(MainTreeItem *item)
{
    childItems.append(item);
}

MainTreeItem *MainTreeItem::child(int row)
{
    return childItems.value(row);
}

QList<MainTreeItem*> MainTreeItem::childrenItems()
{
    return childItems;
}
int MainTreeItem::childCount() const
{
    return childItems.count();
}

int MainTreeItem::columnCount() const
{
    return itemData.count();
}

QVariant MainTreeItem::data(int column) const
{
    return itemData.value(column);
}

QList<QVariant>* MainTreeItem::dataList()
{
    return &itemData;

}

MainTreeItem *MainTreeItem::parent()
{
    return m_parentItem;
}

int MainTreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->childItems.indexOf(const_cast<MainTreeItem*>(this));

    return 0;
}

int MainTreeItem::idNumber() const
{
    return itemId;
}

void MainTreeItem::setIdNumber(int number)
{

    itemId = number;
}
bool MainTreeItem::isExpanded(MainTreeItem::Trees tree) const
{

    if(tree.testFlag(MainTreeItem::DockedTree))
        return m_isExpandedInDockedTree;
        if(tree.testFlag(MainTreeItem::Outliner))
            return m_isExpandedInOutliner;
            if(tree.testFlag(MainTreeItem::Exporter))
                return m_isExpandedInExporter;





    return true;
}

void MainTreeItem::setIsExpanded(bool isExpandedBool, MainTreeItem::Trees tree)
{
    if(tree.testFlag(MainTreeItem::DockedTree))
         m_isExpandedInDockedTree = isExpandedBool;
        if(tree.testFlag(MainTreeItem::Outliner))
             m_isExpandedInOutliner = isExpandedBool;
            if(tree.testFlag(MainTreeItem::Exporter))
                 m_isExpandedInExporter = isExpandedBool;




}

void MainTreeItem::setType(QString type)
{
    m_type = type;
}

QString MainTreeItem::type()
{
    return m_type;
}

MainTreeItem *MainTreeItem::parentItem() const
{
    return m_parentItem;
}

void MainTreeItem::setParentItem(MainTreeItem *parentItem)
{
    m_parentItem = parentItem;
}



bool MainTreeItem::isTrashed() const
{
    return m_isTrashed;
}

void MainTreeItem::setIsTrashed(bool value)
{
    m_isTrashed = value;

}

int MainTreeItem::status() const
{
    return m_status;
}

void MainTreeItem::setStatus(int value)
{
    m_status = value;
}


QString MainTreeItem::badge() const
{
    return m_badge;
}

void MainTreeItem::setBadge(const QString &value)
{
    m_badge = value;
}

Qt::CheckState MainTreeItem::checkState() const
{
    return m_checkState;
}

void MainTreeItem::setCheckState(const Qt::CheckState &checkState)
{
    m_checkState = checkState;
}



QModelIndex MainTreeItem::index() const
{
    return m_index;
}

void MainTreeItem::setIndex(const QModelIndex &index)
{
    m_index = index;
}
