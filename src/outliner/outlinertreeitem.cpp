#include <QStringList>

#include "outliner/outlinertreeitem.h"

OutlinerTreeItem::OutlinerTreeItem(const QList<QVariant> &data, OutlinerTreeItem *parent)
{
    parentItem = parent;
    itemData = data;
    m_povList << "";
    m_isExpanded = true;
}
OutlinerTreeItem::~OutlinerTreeItem()
{
    qDeleteAll(childItems);
}

void OutlinerTreeItem::appendChild(OutlinerTreeItem *item)
{
    childItems.append(item);
}

OutlinerTreeItem *OutlinerTreeItem::child(int row)
{
    return childItems.value(row);
}

int OutlinerTreeItem::childCount() const
{
    return childItems.count();
}

int OutlinerTreeItem::columnCount() const
{
    return itemData.count();
}

QVariant OutlinerTreeItem::data(int column) const
{
    return itemData.value(column);
}

QList<QVariant>* OutlinerTreeItem::dataList()
{
    return &itemData;

}

OutlinerTreeItem *OutlinerTreeItem::parent()
{
    return parentItem;
}

int OutlinerTreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<OutlinerTreeItem*>(this));

    return 0;
}

int OutlinerTreeItem::idNumber() const
{
    return itemId;
}

void OutlinerTreeItem::setIdNumber(int number)
{

    itemId = number;
}
bool OutlinerTreeItem::isExpanded() const
{
    return m_isExpanded;
}

void OutlinerTreeItem::setIsExpanded(bool isExpandedBool)
{
    m_isExpanded = isExpandedBool;
}

