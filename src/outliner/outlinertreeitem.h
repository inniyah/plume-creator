/***************************************************************************
 *   Copyright (C) 2012 by Cyril Jacquet                                   *
 *   cyril.jacquet@plume-creator.eu                                                 *
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
#ifndef OUTLINERTREEITEM_H
#define OUTLINERTREEITEM_H

#include <QString>
#include <QVariant>

#include "hub.h"


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
    bool isExpanded() const;
    void setIsExpanded(bool isExpandedBool);

public slots:
    void setHub(Hub *varHub){hub = varHub;}

private:
    Hub *hub;
   QList<OutlinerTreeItem*> childItems;
    QList<QVariant> itemData;
    OutlinerTreeItem *parentItem;
    int itemId;
    QStringList m_povList;
    bool m_isExpanded;

};

#endif // OUTLINERTREEITEM_H
