/***************************************************************************
 *   Copyright (C) 2012 by Cyril Jacquet                                   *
 *   terreville@gmail.com                                                 *
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
#ifndef ATTENDGLOBALTREEPROXYMODEL_H
#define ATTENDGLOBALTREEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "attendtreeitem.h"
#include "hub.h"

class AttendGlobalTreeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AttendGlobalTreeProxyModel(QObject *parent = 0);
    
    // drag drop :
  Qt::DropActions  supportedDropActions () const;
  QMimeData *mimeData(const QModelIndexList &indexes) const;
  QStringList mimeTypes () const;
    bool dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent );
    Qt::ItemFlags flags(const QModelIndex &index) const;


signals:
    void attendSheetDataNumbersDropped(QList<int> list);

public slots:
    void setHub(Hub *varHub){hub = varHub;}

private:
    Hub *hub;
};

#endif // ATTENDGLOBALTREEPROXYMODEL_H
