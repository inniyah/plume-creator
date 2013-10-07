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
//---------------------------------------------------------------
 bool AttendTreeItem::isExpanded(AttendTreeItem::Trees tree) const
 {

     if(tree.testFlag(AttendTreeItem::ManagerTree))
         return m_isExpandedInManagerTree;
         if(tree.testFlag(AttendTreeItem::GlobalTree))
             return m_isExpandedInGlobalTree;




     return true;
 }

 void AttendTreeItem::setIsExpanded(bool isExpandedBool, AttendTreeItem::Trees tree)
 {
     if(tree.testFlag(AttendTreeItem::ManagerTree))
          m_isExpandedInManagerTree = isExpandedBool;
         if(tree.testFlag(AttendTreeItem::GlobalTree))
              m_isExpandedInGlobalTree = isExpandedBool;




 }
 //---------------------------------------------------------------


 QString AttendTreeItem::name() const
 {
     return m_name;
 }
 void AttendTreeItem::setName(QString name)
 {
    m_name =  name;
 }

 QString AttendTreeItem::aliases() const
 {
     return m_aliases;
 }
 void AttendTreeItem::setAliases(QString aliases)
 {
    m_aliases = aliases;
 }

 QString AttendTreeItem::quickDetails() const
 {
     return m_quickDetails;
 }
 void AttendTreeItem::setQuickDetails(QString details)
 {
    m_quickDetails = details;
 }

 QString AttendTreeItem::box_1Value() const
 {
     return m_box_1Value;
 }


 void AttendTreeItem::setBox_1Value(QString value)
 {
     m_box_1Value = value;
 }


 QString AttendTreeItem::box_2Value() const
 {
     return m_box_2Value;
 }


 void AttendTreeItem::setBox_2Value(QString value)
 {
     m_box_2Value = value;
 }


 QString AttendTreeItem::box_3Value() const
 {
     return m_box_3Value;
 }


 void AttendTreeItem::setBox_3Value(QString value)
 {
     m_box_3Value = value;
 }


 QString AttendTreeItem::spinBox_1_label() const
 {
     return m_spinBox_1_label;
 }


 void AttendTreeItem::setSpinbox_1_label(QString text)
 {
     m_spinBox_1_label = text;
 }


 int AttendTreeItem::spinBox_1Value() const
 {
     return m_spinBox_1Value;
 }


 void AttendTreeItem::setSpinbox_1Value(int value)
 {
     m_spinBox_1Value = value;
 }

 void AttendTreeItem::setType(QString type)
 {
     m_type = type;
 }

 QString AttendTreeItem::type()
 {
     return m_type;
 }
