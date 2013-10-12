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
#ifndef DOCKEDTRASHTREEPROXY_H
#define DOCKEDTRASHTREEPROXY_H

#include <QSortFilterProxyModel>
#include "common/utils.h"
#include "hub.h"
#include "mainTree/maintreeabstractmodel.h"

class DockedTrashTreeProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    DockedTrashTreeProxy(QObject *parent = 0);
    void postConstructor();
    int columnCount(const QModelIndex &parent) const;


    Qt::DropActions supportedDropActions() const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
signals:
    void resetAbsModelSignal();

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void modifyFlagsForDrops(QString indexType);
    void displayBadgeSlot(bool value){badgeIsDisplayed = value;}

private:
    Hub *hub;
QString m_indexTypeDragged;
bool badgeIsDisplayed;

};

#endif // DOCKEDTRASHTREEPROXY_H
