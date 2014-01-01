/***************************************************************************
 *   Copyright (C) 2013 by Cyril Jacquet                                   *
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
#ifndef OUTLINERSPREADSHEETPROXY_H
#define OUTLINERSPREADSHEETPROXY_H

#include <QSortFilterProxyModel>
#include "hub.h"
#include "mainTree/maintreeabstractmodel.h"

class OutlinerSpreadheetProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit OutlinerSpreadheetProxy(QObject *parent = 0);
    void postConstructor();
    ~OutlinerSpreadheetProxy();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);
    QVariant data(const QModelIndex &index, int role) const;

    Qt::DropActions supportedDropActions() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    QStringList mimeTypes() const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

    static QBrush giveTextColor(QModelIndex index);
signals:
    void resetAbsModelSignal();

public slots:
     void setHub(Hub *varHub){hub = varHub;}

     // set the size hint :
     void shrinkRow(){itemHeight += 20; beginResetModel(); endResetModel();}
     void expandRow(){if(itemHeight > 40)itemHeight -= 20; beginResetModel(); endResetModel();}
     void setRowHeight(int height){itemHeight = height; beginResetModel(); endResetModel();}

     void columnOneResizedSlot(int newSize){textZoneColumnOneWidth = newSize;}
     void columnTwoResizedSlot(int newSize){textZoneColumnTwoWidth = newSize;}

     void modifyFlagsForDrops(QString indexType);

private:
    Hub *hub;

    int itemHeight;
    int textZoneColumnOneWidth;
    int textZoneColumnTwoWidth;

    QString m_indexTypeDragged;
};

#endif // OUTLINERSPREADSHEETPROXY_H
