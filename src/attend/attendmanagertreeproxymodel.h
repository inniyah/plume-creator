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
#ifndef ATTENDMANAGERTREEPROXYMODEL_H
#define ATTENDMANAGERTREEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "hub.h"
#include "attendtreeitem.h"

class AttendManagerTreeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AttendManagerTreeProxyModel(QObject *parent = 0);
    void postConstructor();
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void addObject(QModelIndex index);
    void addGroup(QModelIndex index);
    void remove(QModelIndex index);
    void setNameSlot(QModelIndex index, QString name);



    Qt::DropActions supportedDropActions() const;

    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);



signals:
    void resetAbsModelSignal();
    void setNameSignal(QString value);
    void activateItemSignal(QModelIndex index);
    void resetDomElementForNumberSignal();

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void modifyFlagsForDrops(QString indexType);


private:
    Hub *hub;
    void domModified();
    int freeNumber();
    void removeAttendNumberFromSheets(int itemNumber);
    int maxNumber;

    QHash<int, QDomElement> domElementForNumber;


    QString m_indexTypeDragged;

};

#endif // ATTENDMANAGERTREEPROXYMODEL_H
