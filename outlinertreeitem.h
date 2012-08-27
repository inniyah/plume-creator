#ifndef OUTLINERTREEITEM_H
#define OUTLINERTREEITEM_H

#include <QString>
#include <QVariant>

class OutlinerTreeItem
{
public:
    OutlinerTreeItem(const QList<QVariant> &data, OutlinerTreeItem *parent = 0);
    ~OutlinerTreeItem();

    void appendChild(OutlinerTreeItem *child);

    OutlinerTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
  QList<QVariant>* dataList();
  int row() const;
    OutlinerTreeItem *parent();
    int idNumber() const;
    void setIdNumber(int number);

private:
    QList<OutlinerTreeItem*> childItems;
    QList<QVariant> itemData;
    OutlinerTreeItem *parentItem;
    int itemId;

};

#endif // OUTLINERTREEITEM_H
