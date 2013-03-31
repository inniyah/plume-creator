#include <QStringList>

#include "attendtreeitem.h"

AttendTreeItem::AttendTreeItem(const QList<QVariant> &data, AttendTreeItem *parent):
    m_isGroupBool(false)
{
    parentItem = parent;
    itemData = data;

}
AttendTreeItem::~AttendTreeItem()
 {
     qDeleteAll(childItems);
 }

 void AttendTreeItem::appendChild(AttendTreeItem *item)
 {
     childItems.append(item);
 }

 AttendTreeItem *AttendTreeItem::child(int row)
 {
     return childItems.value(row);
 }

 QList<AttendTreeItem*> AttendTreeItem::childrenItems()
 {
     return childItems;
 }

 int AttendTreeItem::childCount() const
 {
     return childItems.count();
 }

 int AttendTreeItem::columnCount() const
 {
     return itemData.count();
 }

 QVariant AttendTreeItem::data(int column) const
 {
     return itemData.value(column);
 }

 QList<QVariant>* AttendTreeItem::dataList()
 {
     return &itemData;

  }

 AttendTreeItem *AttendTreeItem::parent()
 {
     return parentItem;
 }

 int AttendTreeItem::row() const
 {
     if (parentItem)
         return parentItem->childItems.indexOf(const_cast<AttendTreeItem*>(this));

     return 0;
 }

 int AttendTreeItem::idNumber() const
 {
     return itemId;
 }

 void AttendTreeItem::setIdNumber(int number)
 {
     itemId = number;
 }

 bool AttendTreeItem::isGroup() const
 {
     return m_isGroupBool;
 }

 void AttendTreeItem::setIsGroup(bool groupBool)
 {
     m_isGroupBool = groupBool;
 }


 bool AttendTreeItem::isFolded() const
 {
     return m_isFoldedBool;
 }

 void AttendTreeItem::setIsFolded(bool foldedBool)
 {
     m_isFoldedBool = foldedBool;
 }
