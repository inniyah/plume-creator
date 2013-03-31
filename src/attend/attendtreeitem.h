#ifndef ATTENDTREEITEM_H
#define ATTENDTREEITEM_H

#include <QString>
#include <QVariant>

#include "hub.h"


class AttendTreeItem
{
public:
    AttendTreeItem(const QList<QVariant> &data, AttendTreeItem *parent = 0);
    ~AttendTreeItem();

    void appendChild(AttendTreeItem *child);

    AttendTreeItem *child(int row);
    QList<AttendTreeItem*> childrenItems();
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
  QList<QVariant>* dataList();
  int row() const;
    AttendTreeItem *parent();
    int idNumber() const;
    void setIdNumber(int number);
    bool isGroup() const;
    void setIsGroup(bool groupBool);
    bool isFolded() const;
    void setIsFolded(bool foldedBool);

public slots:
    void setHub(Hub *varHub){hub = varHub;}

private:
    Hub *hub;
   QList<AttendTreeItem*> childItems;
    QList<QVariant> itemData;
    AttendTreeItem *parentItem;
    int itemId;
    bool m_isGroupBool;
    bool m_isFoldedBool;


};

#endif // ATTENDTREEITEM_H
