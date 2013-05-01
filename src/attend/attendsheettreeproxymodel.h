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
#ifndef ATTENDSHEETTREEPROXYMODEL_H
#define ATTENDSHEETTREEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "attendtreeitem.h"
#include "hub.h"

class AttendSheetTreeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AttendSheetTreeProxyModel(QObject *parent = 0);

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

      // drag drop :
    Qt::DropActions  supportedDropActions () const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    QStringList mimeTypes () const;
      bool dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent );
      Qt::ItemFlags flags(const QModelIndex &index) const;
      QVariant data(const QModelIndex &index, int role) const;

      protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

signals:
    
public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void currentSheetModified(int sheetNumber);
    void removeSheetObjects(QList<int> objectsList);
    void addSheetObjects(QList<int> objectsList);
    void setPointOfView();
    void setClickedIndex(QModelIndex proxyIndex);

private:
    Hub *hub;
    QList<int> attendList;
    QList<int> povList;
    QHash<int, QDomElement> domElementForNumber;
    QDomElement openedElement;
    QModelIndex clickedSourceIndex;
    QModelIndex clickedProxyIndex;
    bool nothingWasClicked;
    QString sheetName;
};

#endif // ATTENDSHEETTREEPROXYMODEL_H
