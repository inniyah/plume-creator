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
#ifndef ATTENDABSTRACTMODEL_H
#define ATTENDABSTRACTMODEL_H

#include <QObject>
#include <QAbstractItemModel>

#include "hub.h"
#include "attend/attendtreeitem.h"


class AttendAbstractModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit AttendAbstractModel(QObject *parent = 0);
    ~AttendAbstractModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;



    QModelIndex index(int, int, const QModelIndex&) const;
    QModelIndex parent(const QModelIndex&) const;

public slots:
    void setHub(Hub *varHub){hub = varHub;}


signals:
    
public slots:
    void resetAbsModel();

 private slots:
    void parseFolderElement(const QDomElement &element);
    void resetDomDoc();

private:
    static QString createToolTipText(AttendTreeItem *treeItem);
    QString boxListValueAt(QStringList boxList, int index);

    Hub *hub;
    AttendTreeItem *rootItem;


    QDomDocument domDoc;
    QHash<int, QDomElement> domElementForNumber;
    QDomElement root;
    int numberOfDomElements;
    QStringList *titlesList;
    QList<int> *numberList;
    QHash<QTextDocument *, int> numForDoc;

    QList<AttendTreeItem *> *treeGroupItemList;
    QList<AttendTreeItem *> *treeObjectItemList;

};

#endif // ATTENDABSTRACTMODEL_H
