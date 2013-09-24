/***************************************************************************
 *   Copyright (C) 2012 by Cyril Jacquet                                   *
 *   cyril.jacquet@plume-creator.eu                                        *
 *                                                                         *
 *  This file is part of Plume Creator.                                    *
 *                                                                         *
 *  Plume Creator is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  Plume Creator is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with Plume Creator.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/
#ifndef MAINTREEITEM_H
#define MAINTREEITEM_H

#include <QString>
#include <QVariant>
#include <QModelIndex>




class MainTreeItem
{
public:


    enum Tree {
           DockedTree = 0x0,
           Outliner = 0x1,
        Exporter = 0x2,
        FindReplace = 0x4
       };
       Q_DECLARE_FLAGS(Trees, Tree)







    MainTreeItem(const QList<QVariant> &data, MainTreeItem *parent = 0);
    ~MainTreeItem();

    void appendChild(MainTreeItem *child);

    MainTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
  QList<QVariant>* dataList();
  int row() const;
    MainTreeItem *parent();
    int idNumber() const;
    void setIdNumber(int number);

    bool isExpanded(MainTreeItem::Trees trees  = MainTreeItem::DockedTree) const;
    void setIsExpanded(bool isExpandedBool, MainTreeItem::Trees trees  = MainTreeItem::DockedTree);

    void setType(QString type);
    QString type();
    QList<MainTreeItem *> childrenItems();
    MainTreeItem *parentItem() const;
    void setParentItem(MainTreeItem *parentItem);

    bool isTrashed() const;
    void setIsTrashed(bool value);

    int status() const;
    void setStatus(int value);

    QString badge() const;
    void setBadge(const QString &value);

    Qt::CheckState checkState(MainTreeItem::Trees trees  = MainTreeItem::DockedTree) const;
    void setCheckState(const Qt::CheckState &checkState, MainTreeItem::Trees trees  = MainTreeItem::DockedTree);



    QModelIndex index() const;
    void setIndex(const QModelIndex &index);

public slots:


private:

    QList<MainTreeItem*> childItems;
    QList<QVariant> itemData;
    MainTreeItem *m_parentItem;
    int itemId;
    QStringList m_povList;
    bool m_isExpandedInDockedTree, m_isExpandedInOutliner, m_isExpandedInExporter, m_isExpandedInFindReplace;
    Qt::CheckState m_exporterCheckState, m_findReplaceCheckState;
    QString m_type;
    bool m_isTrashed;
    int m_status;
    QString m_badge;
    QModelIndex m_index;

};
Q_DECLARE_OPERATORS_FOR_FLAGS(MainTreeItem::Trees)

#endif // MAINTREEITEM_H
