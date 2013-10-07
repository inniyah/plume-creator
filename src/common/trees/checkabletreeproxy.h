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
#ifndef CHECKABLETREEPROXY_H
#define CHECKABLETREEPROXY_H

#include <QSortFilterProxyModel>
#include "utils.h"
#include "hub.h"
#include "mainTree/maintreeitem.h"
#include "mainTree/maintreeabstractmodel.h"



class CheckableTreeProxy : public QSortFilterProxyModel
{


Q_OBJECT

public:
    enum TreeFlag {
        NoOption = 0x0,
        SeparatorCheckable = 0x1

       };
       Q_DECLARE_FLAGS(TreeFlags, TreeFlag)


    explicit CheckableTreeProxy(QObject *parent = 0, QString expandElementAttributeName = "exporterExpanded",
                               QString checkStateElementAttributeName = "exporterCheckState",
                                MainTreeItem::Tree tree = MainTreeItem::Exporter);
    void postConstructor();
    int columnCount(const QModelIndex &parent) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    void setColumnCount(int count){m_columnCount = count;}


    void setNumberForSheetIdHash(const QHash<int, int> &value);

    void setTreeFlags(CheckableTreeProxy::TreeFlags optionFlag){treeFlag = optionFlag;}
    CheckableTreeProxy::TreeFlags treeFlags(){return treeFlag;}


signals:

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void checkBoxChanged(QModelIndex index, Qt::CheckState state);
    void setSecondaryOpenedSheetSlot(int id);

private slots:

    void setChildrenCheckState(QModelIndex index, Qt::CheckState state);
    void setParentCheckState(QModelIndex index, Qt::CheckState state, int idToForgetIfPartiallyChecked = -1);


private:
    Hub *hub;
QString m_indexTypeDragged;
bool badgeIsDisplayed;
QString expandAttributeName, checkStateAttributeName;
MainTreeItem::Tree treeType;

int m_columnCount;
QHash<int,int> numberForSheetIdHash;
CheckableTreeProxy::TreeFlags treeFlag;

int secondaryOpenedSheet;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CheckableTreeProxy::TreeFlags)

#endif // CHECKABLETREEPROXY_H
